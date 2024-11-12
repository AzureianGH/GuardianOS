#include <libhydrix/hio/io.h>
//PortIO::InByte and PortIO::OutByte
uint8_t PortIO::InByte(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void PortIO::OutByte(uint16_t port, uint8_t data) {
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(port));
}

//PortIO::InWord and PortIO::OutWord
uint16_t PortIO::InWord(uint16_t port) {
    uint16_t ret;
    __asm__ __volatile__("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void PortIO::OutWord(uint16_t port, uint16_t data) {
    __asm__ __volatile__("outw %0, %1" : : "a"(data), "Nd"(port));
}

//PortIO::InLong and PortIO::OutLong

uint32_t PortIO::InLong(uint16_t port) {
    uint32_t ret;
    __asm__ __volatile__("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void PortIO::OutLong(uint16_t port, uint32_t data) {
    __asm__ __volatile__("outl %0, %1" : : "a"(data), "Nd"(port));
}

//PortIO::InSL and PortIO::OutSL
void PortIO::InSL(uint16_t port, void* buffer, uint32_t count) {
    __asm__ __volatile__("rep insb" : "+D"(buffer), "+c"(count) : "d"(port) : "memory");
}

void PortIO::OutSL(uint16_t port, void* buffer, uint32_t count) {
    __asm__ __volatile__("rep outsb" : "+S"(buffer), "+c"(count) : "d"(port) : "memory");
}

//ioPITWait
void PortIO::IoPITWait(void) {
    InByte(0x80);
}
