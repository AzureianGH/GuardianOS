#pragma once
#include <stdint.h>
#include <stddef.h>

class PortIO
{
    public:
    /// @brief Read a byte from a port
    /// @param port The port
    /// @return The byte read
    static uint8_t InByte(uint16_t port);
    /// @brief Write a byte to a port
    /// @param port The port
    /// @param data The byte to write
    static void OutByte(uint16_t port, uint8_t data);
    /// @brief Read a word from a port
    /// @param port The port
    /// @return The word read
    static uint16_t InWord(uint16_t port);
    /// @brief Write a word to a port
    /// @param port The port
    /// @param data The word to write
    static void OutWord(uint16_t port, uint16_t data);
    /// @brief Read a double word from a port
    /// @param port The port
    /// @return The double word read
    static uint32_t InLong(uint16_t port);
    /// @brief Write a double word to a port
    /// @param port The port
    static void OutLong(uint16_t port, uint32_t data);
    /// @brief Read a byte from a port with string load
    /// @param port The port
    /// @param buffer The buffer to read into
    /// @param count The number of bytes to read
    static void InSL(uint16_t port, void* buffer, uint32_t count);
    /// @brief Write a byte to a port with string store
    /// @param port The port
    /// @param buffer The buffer to write from
    /// @param count The number of bytes to write
    static void OutSL(uint16_t port, void* buffer, uint32_t count);
    /// @brief PITWait for I/O to complete
    static void IoPITWait(void);
};