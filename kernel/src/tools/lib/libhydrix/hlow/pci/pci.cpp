#include <libhydrix/hlow/pci/pci.h>
#include <libhydrix/hio/io.h>
#include <libhydrix/hmem/smem/heap.h>
#include <libhydrix/hcon/console.h>

pci_device **pci_devices = 0;
uint32_t devs = 0;

pci_driver **pci_drivers = 0;
uint32_t drivs = 0;
Console *con_pci;

void SetPCIConsole(Console *con)
{
    con_pci = con;
}

void add_pci_device(pci_device *pdev)
{
	pci_devices[devs] = pdev;
	devs ++;
	return;
}

uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset)
{
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000)); // Get the address 
    PortIO::OutLong(0xCF8, address);
    tmp = (uint16_t)((PortIO::InLong (0xCFC) >> ((offset & 2) * 8)) & 0xffff); // Read the data
    return (tmp);
}

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function)
{
    return pci_read_word(bus, device, function, 0);
}

uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function)
{
    return pci_read_word(bus, device, function, 2);
}

uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function)
{
    return (pci_read_word(bus, device, function, 0xA) & 0xFF00) >> 8;
}

uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function)
{
    return pci_read_word(bus, device, function, 0xA) & 0x00FF;
}

const char* pci_get_class_name(uint16_t class_code, uint16_t subclass_code)
{
    switch (class_code)
    {
        case 0x01:
            switch (subclass_code)
            {
                case 0x01: return "IDE Controller";
                case 0x06: return "SATA Controller";
                default: return "Mass Storage Controller";
            }
        case 0x02:
            switch (subclass_code)
            {
                case 0x00: return "Ethernet Controller";
                default: return "Network Controller";
            }
        case 0x03:
            switch (subclass_code)
            {
                case 0x00: return "VGA-Compatible Controller";
                default: return "Display Controller";
            }
        case 0x04: return "Multimedia Controller";
        case 0x06: return "Bridge Device";
        case 0x0C:
            switch (subclass_code)
            {
                case 0x03: return "USB Controller";
                default: return "Serial Bus Controller";
            }
        default: return "Unknown Device";
    }
}

void pci_probe()
{
    for(uint32_t bus = 0; bus < 256; bus++)
    {
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            for(uint32_t function = 0; function < 8; function++)
            {
                uint16_t vendor = getVendorID(bus, slot, function);
                if (vendor == 0xFFFF) continue;
                
                uint16_t device = getDeviceID(bus, slot, function);
                uint16_t class_code = getClassId(bus, slot, function);
                uint16_t subclass_code = getSubClassId(bus, slot, function);

                // Log the device
                const char* class_name = pci_get_class_name(class_code, subclass_code);
                
                pci_device *pdev = (pci_device *)KernelAllocate(sizeof(pci_device));
                pdev->vendor = vendor;
                pdev->device = device;
                pdev->func = function;
                pdev->driver = 0;
                pdev->type = (char*)class_name;
                add_pci_device(pdev);
            }
        }
    }
}

void InitializePCI()
{
	devs = drivs = 0;
	pci_devices = (pci_device **)KernelAllocate(32 * sizeof(pci_device));
	pci_drivers = (pci_driver **)KernelAllocate(32 * sizeof(pci_driver));
	pci_probe();
    return;
}

void pci_register_driver(pci_driver *driv)
{
	pci_drivers[drivs] = driv;
	drivs++;
	return;
}

pci_device **GetPCIDevices()
{
    return pci_devices;
}

uint32_t GetPCIDeviceCount()
{
    return devs;
}

pci_driver** GetPCIDrivers()
{
    return pci_drivers;
}
