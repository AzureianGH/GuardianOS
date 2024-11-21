#include <libhydrix/hgl/display.h>
#include <libhydrix/libhydrix.h>
EDID_Information_t* ParseEDID(void *data)
{
    EDID_Information_t* info = (EDID_Information_t*)KernelAllocate(sizeof(EDID_Information_t));
    memcpy(info, data, sizeof(EDID_Information_t));
    return info;
}

//  	Checksum (Low Byte of 16-bit sum of 00-7Eh) 

uint8_t CalculateEDIDChecksum(EDID_Information_t* edid)
{
    uint8_t sum = 0;
    for (int i = 0; i < 127; i++)
    {
        sum += ((uint8_t*)edid)[i];
    }
    return 0x100 - sum; // Get the low byte of the sum
}

bool ValidateEDIDChecksum(EDID_Information_t* edid)
{
    return edid->checksum == CalculateEDIDChecksum(edid);
}