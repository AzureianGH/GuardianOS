#include <libhydrix/hlow/cpu/cpuchecks.h>
#include <stdint.h>
#include <stddef.h>
#include <libhydrix/hmem/smem/heap.h>
void CPUSupport::CPUID(int code, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d)
{
    asm volatile("cpuid"
                 : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                 : "a"(code));
}

// Check if the CPU supports a feature
bool CPUSupport::CPUSupportCheckFor(CPUSupport::CPUFeature feature, bool Extended = false)
{
    //extended is for checking the edx register
    unsigned int a, b, c, d;
    if (Extended)
    {
        CPUID(0x80000001, &a, &b, &c, &d);
        return (d & (1 << (int)feature));
    }
    else
    {
        CPUID(1, &a, &b, &c, &d);
        return (c & (1 << (int)feature));
    }
}

char *CPUSupport::GetRawCPUVendor()
{
    unsigned int a, b, c, d;
    char *vendor = (char *)KernelAllocate(13);
    CPUID(0, &a, &b, &c, &d);
    *(uint32_t *)vendor = b;
    *(uint32_t *)(vendor + 4) = d;
    *(uint32_t *)(vendor + 8) = c;
    vendor[12] = '\0';
    return vendor;
}

char *CPUSupport::GetRawCPUName()
{
    //Instead of using something like AuthenticAMD, we can use the brand string
    unsigned int a, b, c, d;
    char *brand = (char *)KernelAllocate(49);
    CPUID(0x80000002, &a, &b, &c, &d);
    *(uint32_t *)brand = a;
    *(uint32_t *)(brand + 4) = b;
    *(uint32_t *)(brand + 8) = c;
    *(uint32_t *)(brand + 12) = d;
    CPUID(0x80000003, &a, &b, &c, &d);
    *(uint32_t *)(brand + 16) = a;
    *(uint32_t *)(brand + 20) = b;
    *(uint32_t *)(brand + 24) = c;
    *(uint32_t *)(brand + 28) = d;
    CPUID(0x80000004, &a, &b, &c, &d);
    *(uint32_t *)(brand + 32) = a;
    *(uint32_t *)(brand + 36) = b;
    *(uint32_t *)(brand + 40) = c;
    *(uint32_t *)(brand + 44) = d;
    brand[48] = '\0';
    return brand;
}

//list of the cpu strings in a list, so AuthenicAMD would be AMD, and GenuineIntel would be Intel

char *CPUSupport::GetCPUVendor()
{
    char *vendor = GetRawCPUVendor();
    if (vendor[0] == 'A' && vendor[1] == 'u' && vendor[2] == 't' && vendor[3] == 'h' && vendor[4] == 'e' && vendor[5] == 'n' && vendor[6] == 't' && vendor[7] == 'i' && vendor[8] == 'c' && vendor[9] == 'A' && vendor[10] == 'M' && vendor[11] == 'D')
    {
        return "AMD";
    }
    // CentaurHauls
    else if (vendor[0] == 'C' && vendor[1] == 'e' && vendor[2] == 'n' && vendor[3] == 't' && vendor[4] == 'a' && vendor[5] == 'u' && vendor[6] == 'r' && vendor[7] == 'H' && vendor[8] == 'a' && vendor[9] == 'u' && vendor[10] == 'l' && vendor[11] == 's')
    {
        return "WinChip/Centaur";
    }
    // CyrixInstead
    else if (vendor[0] == 'C' && vendor[1] == 'y' && vendor[2] == 'r' && vendor[3] == 'i' && vendor[4] == 'x' && vendor[5] == 'I' && vendor[6] == 'n' && vendor[7] == 's' && vendor[8] == 't' && vendor[9] == 'e' && vendor[10] == 'a' && vendor[11] == 'd')
    {
        return "Cyrix";
    }
    // GenuineIntel
    else if (vendor[0] == 'G' && vendor[1] == 'e' && vendor[2] == 'n' && vendor[3] == 'u' && vendor[4] == 'i' && vendor[5] == 'n' && vendor[6] == 'e' && vendor[7] == 'I' && vendor[8] == 'n' && vendor[9] == 't' && vendor[10] == 'e' && vendor[11] == 'l')
    {
        return "Intel";
    }
    // GenuineIotel (Iotel is a typo)
    else if (vendor[0] == 'G' && vendor[1] == 'e' && vendor[2] == 'n' && vendor[3] == 'u' && vendor[4] == 'i' && vendor[5] == 'n' && vendor[6] == 'e' && vendor[7] == 'I' && vendor[8] == 'o' && vendor[9] == 't' && vendor[10] == 'e' && vendor[11] == 'l')
    {
        return "Intel";
    }
    // TransmetaCPU
    else if (vendor[0] == 'T' && vendor[1] == 'r' && vendor[2] == 'a' && vendor[3] == 'n' && vendor[4] == 's' && vendor[5] == 'm' && vendor[6] == 'e' && vendor[7] == 't' && vendor[8] == 'a' && vendor[9] == 'C' && vendor[10] == 'P' && vendor[11] == 'U')
    {
        return "Transmeta";
    }
    // GenuineTMx86
    else if (vendor[0] == 'G' && vendor[1] == 'e' && vendor[2] == 'n' && vendor[3] == 'u' && vendor[4] == 'i' && vendor[5] == 'n' && vendor[6] == 'e' && vendor[7] == 'T' && vendor[8] == 'M' && vendor[9] == 'x' && vendor[10] == '8' && vendor[11] == '6')
    {
        return "Transmeta";
    }
    // Geode by NSC
    else if (vendor[0] == 'G' && vendor[1] == 'e' && vendor[2] == 'o' && vendor[3] == 'd' && vendor[4] == 'e' && vendor[5] == 'b' && vendor[6] == 'y' && vendor[7] == 'N' && vendor[8] == 'S' && vendor[9] == 'C')
    {
        return "National Semiconductor";
    }
    // NexGenDriven
    else if (vendor[0] == 'N' && vendor[1] == 'e' && vendor[2] == 'x' && vendor[3] == 'G' && vendor[4] == 'e' && vendor[5] == 'n' && vendor[6] == 'D' && vendor[7] == 'r' && vendor[8] == 'i' && vendor[9] == 'v' && vendor[10] == 'e' && vendor[11] == 'n')
    {
        return "NexGen";
    }
    // RiseRiseRise
    else if (vendor[0] == 'R' && vendor[1] == 'i' && vendor[2] == 's' && vendor[3] == 'e' && vendor[4] == 'R' && vendor[5] == 'i' && vendor[6] == 's' && vendor[7] == 'e' && vendor[8] == 'R' && vendor[9] == 'i' && vendor[10] == 's' && vendor[11] == 'e')
    {
        return "Rise";
    }
    // SiS SiS SiS
    else if (vendor[0] == 'S' && vendor[1] == 'i' && vendor[2] == 'S' && vendor[3] == ' ' && vendor[4] == 'S' && vendor[5] == 'i' && vendor[6] == 'S' && vendor[7] == ' ' && vendor[8] == 'S' && vendor[9] == 'i' && vendor[10] == 'S')
    {
        return "SiS";
    }
    // UMC UMC UMC
    else if (vendor[0] == 'U' && vendor[1] == 'M' && vendor[2] == 'C' && vendor[3] == ' ' && vendor[4] == 'U' && vendor[5] == 'M' && vendor[6] == 'C' && vendor[7] == ' ' && vendor[8] == 'U' && vendor[9] == 'M' && vendor[10] == 'C')
    {
        return "UMC";
    }
    // Vortex86 SoC
    else if (vendor[0] == 'V' && vendor[1] == 'o' && vendor[2] == 'r' && vendor[3] == 't' && vendor[4] == 'e' && vendor[5] == 'x' && vendor[6] == '8' && vendor[7] == '6' && vendor[8] == ' ' && vendor[9] == 'S' && vendor[10] == 'o' && vendor[11] == 'C')
    {
        return "DM&P Vortex86";
    }
    // Shanghai
    else if (vendor[2] == 'S' && vendor[3] == 'h' && vendor[4] == 'a' && vendor[5] == 'n' && vendor[6] == 'g' && vendor[7] == 'h' && vendor[8] == 'a' && vendor[9] == 'i')
    {
        return "Zhaoxin";
    }
    // HygonGenuine
    else if (vendor[0] == 'H' && vendor[1] == 'y' && vendor[2] == 'g' && vendor[3] == 'o' && vendor[4] == 'n' && vendor[5] == 'G' && vendor[6] == 'e' && vendor[7] == 'n' && vendor[8] == 'u' && vendor[9] == 'i' && vendor[10] == 'n' && vendor[11] == 'e')
    {
        return "Hygon";
    }
    // Genuine RDC
    else if (vendor[0] == 'G' && vendor[1] == 'e' && vendor[2] == 'n' && vendor[3] == 'u' && vendor[4] == 'i' && vendor[5] == 'n' && vendor[6] == 'e' && vendor[7] == ' ' && vendor[8] == 'R' && vendor[9] == 'D' && vendor[10] == 'C')
    {
        return "RDC Semiconductor Co. Ltd.";
    }
    // E2K MACHINE
    else if (vendor[0] == 'E' && vendor[1] == '2' && vendor[2] == 'K' && vendor[3] == ' ' && vendor[4] == 'M' && vendor[5] == 'A' && vendor[6] == 'C' && vendor[7] == 'H' && vendor[8] == 'I' && vendor[9] == 'N' && vendor[10] == 'E')
    {
        return "MCST Elbrus";
    }
    // VIA VIA VIA
    else if (vendor[0] == 'V' && vendor[1] == 'I' && vendor[2] == 'A' && vendor[3] == ' ' && vendor[4] == 'V' && vendor[5] == 'I' && vendor[6] == 'A' && vendor[7] == ' ' && vendor[8] == 'V' && vendor[9] == 'I' && vendor[10] == 'A')
    {
        return "VIA";
    }
    // AMD ISBETTER
    else if (vendor[0] == 'A' && vendor[1] == 'M' && vendor[2] == 'D' && vendor[3] == ' ' && vendor[4] == 'I' && vendor[5] == 'S' && vendor[6] == 'B' && vendor[7] == 'E' && vendor[8] == 'T' && vendor[9] == 'T' && vendor[10] == 'E' && vendor[11] == 'R')
    {
        return "AMD";
    }
    // VmwareVmware
    else if (vendor[0] == 'V' && vendor[1] == 'm' && vendor[2] == 'w' && vendor[3] == 'a' && vendor[4] == 'r' && vendor[5] == 'e' && vendor[6] == 'V' && vendor[7] == 'm' && vendor[8] == 'w' && vendor[9] == 'a' && vendor[10] == 'r' && vendor[11] == 'e')
    {
        return "Vmware";
    }
    // KVMKVMKVM
    else if (vendor[0] == 'K' && vendor[1] == 'V' && vendor[2] == 'M' && vendor[3] == 'K' && vendor[4] == 'V' && vendor[5] == 'M' && vendor[6] == 'K' && vendor[7] == 'V' && vendor[8] == 'M' && vendor[9] == 'K' && vendor[10] == 'V' && vendor[11] == 'M')
    {
        return "KVM";
    }
    // Microsoft Hv
    else if (vendor[0] == 'M' && vendor[1] == 'i' && vendor[2] == 'c' && vendor[3] == 'r' && vendor[4] == 'o' && vendor[5] == 's' && vendor[6] == 'o' && vendor[7] == 'f' && vendor[8] == 't' && vendor[9] == ' ' && vendor[10] == 'H' && vendor[11] == 'v')
    {
        return "Microsoft Hypervisor";
    }
    //otherwise, return the raw vendor
    return vendor;
}