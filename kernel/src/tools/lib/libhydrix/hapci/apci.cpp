#include <libhydrix/hapci/acpi.h>
Console* consoleacpi;

dword *SMI_CMD;
byte ACPI_ENABLE;
byte ACPI_DISABLE;
dword *PM1a_CNT;
dword *PM1b_CNT;
word SLP_TYPa;
word SLP_TYPb;
word SLP_EN;
word SCI_EN;
byte PM1_CNT_LEN;

struct RSDPtr
{
    byte Signature[8];
    byte CheckSum;
    byte OemID[6];
    byte Revision;
    dword *RsdtAddress;
};

struct FACP
{
    byte Signature[4];
    dword Length;
    byte unneded1[40 - 8];
    dword *DSDT;
    byte unneded2[48 - 44];
    dword *SMI_CMD;
    byte ACPI_ENABLE;
    byte ACPI_DISABLE;
    byte unneded3[64 - 54];
    dword *PM1a_CNT_BLK;
    dword *PM1b_CNT_BLK;
    byte unneded4[89 - 72];
    byte PM1_CNT_LEN;
};

void SetACPIConsole(Console* console_)
{
    consoleacpi = console_;
}

// Check if the given address has a valid header
unsigned int* acpiCheckRSDPtr(unsigned int* ptr)
{
    char* sig = "RSD PTR ";
    struct RSDPtr* rsdp = (struct RSDPtr*) ptr;
    byte* bptr;
    byte check = 0;
    int i;

    if (memcmp(sig, rsdp, 8) == 0)
    {
        // Check checksum rsdpd
        bptr = (byte*) ptr;
        for (i = 0; i < sizeof(struct RSDPtr); i++)
        {
            check += *bptr;
            bptr++;
        }

        // Found valid rsdpd
        if (check == 0) {
            return (unsigned int*) rsdp->RsdtAddress;
        }
    }

    return NULL;
}

// Finds the ACPI header and returns the address of the RSDT
unsigned int* acpiGetRSDPtr(void)
{
    unsigned int* addr;
    unsigned int* rsdp;

    // Search below the 1MB mark for RSDP signature
    for (addr = (unsigned int*) 0x000E0000; (uintptr_t) addr < 0x00100000; addr += 0x10 / sizeof(*addr))
    {
        rsdp = acpiCheckRSDPtr(addr);
        if (rsdp != NULL)
            return rsdp;
    }

    // At address 0x40:0x0E is the RM segment of the EBDA
    int ebda = *((short*) 0x40E);    // Get pointer
    ebda = (ebda * 0x10) & 0x000FFFFF;    // Transform segment into linear address

    // Search Extended BIOS Data Area for the Root System Description Pointer signature
    for (addr = (unsigned int*) ebda; (uintptr_t) addr < ebda + 1024; addr += 0x10 / sizeof(*addr))
    {
        rsdp = acpiCheckRSDPtr(addr);
        if (rsdp != NULL)
            return rsdp;
    }

    return NULL;
}

// Checks for a given header and validates checksum
int acpiCheckHeader(unsigned int* ptr, char* sig)
{
    if (memcmp(ptr, sig, 4) == 0)
    {
        char* checkPtr = (char*) ptr;
        int len = *(ptr + 1);
        char check = 0;
        while (0 < len--)
        {
            check += *checkPtr;
            checkPtr++;
        }
        if (check == 0)
            return 0;
    }
    return -1;
}
string ReasonForFailure = "";
int acpiEnable(void)
{
    // Check if ACPI is enabled
    if ((PortIO::InWord(reinterpret_cast<uintptr_t>(PM1a_CNT)) & SCI_EN) == 0)
    {
        // Check if ACPI can be enabled
        if (SMI_CMD != nullptr && ACPI_ENABLE != 0)
        {
            PortIO::OutByte(reinterpret_cast<uintptr_t>(SMI_CMD), ACPI_ENABLE); // Send ACPI enable command
            // Give 3 seconds time to enable ACPI
            int i;
            for (i = 0; i < 300; i++)
            {
                if ((PortIO::InWord(reinterpret_cast<uintptr_t>(PM1a_CNT)) & SCI_EN) == 1)
                    break;
                PITSleepMS(10);
            }
            if (PM1b_CNT != nullptr)
                for (; i < 300; i++)
                {
                    if ((PortIO::InWord(reinterpret_cast<uintptr_t>(PM1b_CNT)) & SCI_EN) == 1)
                        break;
                    PITSleepMS(10);
                }
            if (i < 300) {
                return 0;
            }
            else {
                ReasonForFailure = "Couldn't Enable ACPI.";
                return -1;
            }
        }
        else {
            ReasonForFailure = "No Known Way To Enable ACPI.";
            return -1;
        }
    }
    else {
        //console->WriteLineS("ACPI was already enabled.\n");
        return 0;
    }
}
string GetReasonForFailureACPI()
{
    return ReasonForFailure;
}
int initAcpi(void)
{
    unsigned int* ptr = acpiGetRSDPtr();

    // Check if address is correct (if ACPI is available on this PC)
    if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
    {
        // The RSDT contains an unknown number of pointers to ACPI tables
        int entrys = *(ptr + 1);
        entrys = (entrys - 36) / 4;
        ptr += 36 / 4;    // Skip header information

        while (0 < entrys--)
        {
            // Check if the desired table is reached
            if (acpiCheckHeader((unsigned int*) *ptr, "FACP") == 0)
            {
                entrys = -2;
                struct FACP* facp = (struct FACP*) *ptr;
                if (acpiCheckHeader((unsigned int*) facp->DSDT, "DSDT") == 0)
                {
                    // Search the \_S5 package in the DSDT
                    char* S5Addr = (char*) facp->DSDT + 36; // Skip header
                    int dsdtLength = *(facp->DSDT + 1) - 36;
                    while (0 < dsdtLength--)
                    {
                        if (memcmp(S5Addr, "_S5_", 4) == 0)
                            break;
                        S5Addr++;
                    }
                    // Check if \_S5 was found
                    if (dsdtLength > 0)
                    {
                        // Check for valid AML structure
                        if ((*(S5Addr - 1) == 0x08 || (*(S5Addr - 2) == 0x08 && *(S5Addr - 1) == '\\')) && *(S5Addr + 4) == 0x12)
                        {
                            S5Addr += 5;
                            S5Addr += ((*S5Addr & 0xC0) >> 6) + 2;    // Calculate PkgLength size

                            if (*S5Addr == 0x0A)
                                S5Addr++;    // Skip byteprefix
                            SLP_TYPa = *(S5Addr) << 10;
                            S5Addr++;

                            if (*S5Addr == 0x0A)
                                S5Addr++;    // Skip byteprefix
                            SLP_TYPb = *(S5Addr) << 10;

                            SMI_CMD = facp->SMI_CMD;

                            ACPI_ENABLE = facp->ACPI_ENABLE;
                            ACPI_DISABLE = facp->ACPI_DISABLE;

                            PM1a_CNT = facp->PM1a_CNT_BLK;
                            PM1b_CNT = facp->PM1b_CNT_BLK;

                            PM1_CNT_LEN = facp->PM1_CNT_LEN;

                            SLP_EN = 1 << 13;
                            SCI_EN = 1;

                            return 0;
                        }
                        else {
                            ReasonForFailure = "\\_S5 Parse Error.";
                        }
                    }
                    else {
                        ReasonForFailure = "\\_S5 Not Present.";
                    }
                }
                else {
                    ReasonForFailure = "DSDT Invalid.";
                }
            }
            ptr++;
        }
        ReasonForFailure = "No Valid FACP Present.";
    }
    else {
        ReasonForFailure = "No ACPI.";
    }

    return -1;
}

void acpiPowerOff(void)
{
    // SCI_EN is set to 1 if ACPI shutdown is possible
    if (SCI_EN == 0)
        return;

    acpiEnable();

    // Send the shutdown command
    PortIO::OutWord(reinterpret_cast<uintptr_t>(PM1a_CNT), SLP_TYPa | SLP_EN);
    if (PM1b_CNT != nullptr)
        PortIO::OutWord(reinterpret_cast<uintptr_t>(PM1b_CNT), SLP_TYPb | SLP_EN);

    consoleacpi->WriteLineS("ACPI poweroff failed.\n", IColor::RGB(255, 0, 0));
}