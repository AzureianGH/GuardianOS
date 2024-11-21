#pragma once
#include <stdint.h>
#include <stddef.h>
#pragma pack(push, 1) // Disable padding
typedef struct 
{
    uint8_t padding[8];                     // 00h: 8 Bytes Padding
    uint16_t manufacture_id;               // 08h: 1 Word Manufacture ID (Big-Endian)
    uint16_t edid_id_code;                 // 0Ah: 1 Word EDID ID code
    uint32_t serial_number;                // 0Ch: 1 DWord Serial Number
    uint8_t manufacture_week;              // 10h: 1 Byte Manufacture Week
    uint8_t manufacture_year;              // 11h: 1 Byte Manufacture Year
    uint8_t edid_version;                  // 12h: 1 Byte EDID Version
    uint8_t edid_revision;                 // 13h: 1 Byte EDID Revision
    uint8_t video_input_type;              // 14h: 1 Byte Video Input Type
    uint8_t max_horizontal_size;           // 15h: 1 Byte Max Horizontal Size (cm)
    uint8_t max_vertical_size;             // 16h: 1 Byte Max Vertical Size (cm)
    uint8_t gamma_factor;                  // 17h: 1 Byte Gamma Factor
    uint8_t dpms_flags;                    // 18h: 1 Byte DPMS Flags
    uint8_t chroma_information[10];        // 19h: 10 Bytes Chroma Information
    uint8_t established_timings_1;         // 23h: 1 Byte Established Timings 1
    uint8_t established_timings_2;         // 24h: 1 Byte Established Timings 2
    uint8_t manufacturers_reserved_timings;// 25h: 1 Byte Manufacturer's Reserved Timings
    uint16_t standard_timing_identification[8]; // 26h: 8 Words Standard Timing Identification
    uint8_t detailed_timing_description_1[18];  // 36h: 18 Bytes Detailed Timing Description 1
    uint8_t detailed_timing_description_2[18];  // 48h: 18 Bytes Detailed Timing Description 2
    uint8_t detailed_timing_description_3[18];  // 5Ah: 18 Bytes Detailed Timing Description 3
    uint8_t detailed_timing_description_4[18];  // 6Ch: 18 Bytes Detailed Timing Description 4
    uint8_t unused;                        // 7Eh: 1 Byte Unused
    uint8_t checksum;                      // 7Fh: 1 Byte Checksum
} EDID_Information_t;
#pragma pack(pop)

EDID_Information_t* ParseEDID(void* data);
bool ValidateEDIDChecksum(EDID_Information_t* edid);