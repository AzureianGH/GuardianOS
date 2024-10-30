#pragma once
#include <stdint.h>
#include <stddef.h>
#include <libhydrix/hmem/smem/heap.h>
/// @brief A function pointer that can accept any amount of arguments and return any type.
typedef void (*function_pointer)();
/// @brief Basic handle in general.
#define HANDLE void*
/// @brief An address in memory.
#define ADDRESS void*
/// @brief A Point, with x and y coordinates.
typedef struct _point {
    /// @brief The x coordinate.
    int x;
    /// @brief The y coordinate.
    int y;
} Point;

/// @brief A Floating Point, with x and y coordinates.
typedef struct _fpoint {
    /// @brief The x coordinate.
    float x;
    /// @brief The y coordinate.
    float y;
} FPoint;

/// @brief A Rectangle, with x, y, width, and height.
typedef struct _rect {
    /// @brief The x coordinate.
    int x;
    /// @brief The y coordinate.
    int y;
    /// @brief The width.
    int w;
    /// @brief The height.
    int h;
} Rect;

/// @brief A Floating Rectangle, with x, y, width, and height.
typedef struct _frect {
    /// @brief The x coordinate.
    float x;
    /// @brief The y coordinate.
    float y;
    /// @brief The width.
    float w;
    /// @brief The height.
    float h;
} FRect;

/// @brief A Size, with width and height.
typedef struct _size {
    /// @brief The width.
    int w;
    /// @brief The height.
    int h;
} Size;

/// @brief A Floating Size, with width and height.
typedef struct _fsize {
    /// @brief The width.
    float w;
    /// @brief The height.
    float h;
} FSize;

/// @brief A Color
typedef struct _color {
    /// @brief The red value.
    uint8_t r;
    /// @brief The green value.
    uint8_t g;
    /// @brief The blue value.
    uint8_t b;
} Color;

/// @brief A Color with alpha
typedef struct _colora {
    /// @brief The red value.
    uint8_t r;
    /// @brief The green value.
    uint8_t g;
    /// @brief The blue value.
    uint8_t b;
    /// @brief The alpha value.
    uint8_t a;
} ColorA;

/// @brief A Bitmap
typedef struct _bmp
{
    uint8_t* data;
    int width;
    int height;
} BMP;

/// @brief A Bitmap (int)
typedef struct _bmpi
{
    int* data;
    int width;
    int height;
} BMPI;

/// @brief A Bitmap (Alpha) (int)
typedef struct _bmpa
{
    long* data; //Must be long because if an int had 0xFFFFFFFF it would be -1
    int width;
    int height;
} BMPA;

typedef uint8_t byte;

BMPI StretchBMPI(BMPI bmp, int width, int height);
BMPA StretchBMPA(BMPA bmp, int width, int height);

//word, dword, qword
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;
#define nullptr 0

#define VECTOR_RESIZE_FACTOR 2

//Vector template
template <typename T> class Vector
{
public:
    Vector()
    {
        data = nullptr;
        len = 0;
        capacity = 0;
    }

    ~Vector()
    {
        if (data != nullptr)
        {
            KernelFree(data);
        }
    }

    void PushBack(T elem)
    {
        if (len == capacity)
        {
            capacity = capacity ? capacity * VECTOR_RESIZE_FACTOR : 1;
            data = (T*)KernelReallocate(data, sizeof(data[0]) * capacity);
        }
        data[len++] = elem;
    }

    T PopBack()
    {
        //remove the last element
        T elem = data[len - 1];
        len--;
        return elem;
    }
    size_t Length()
    {
        return len;
    }

    T At(size_t index)
    {
        return data[index];
    }

    void Erase(size_t index)
    {
        memcpy(&data[index], &data[index + 1], sizeof(data[0]) * len - index - 1);
        len--;
    }

    void EraseAll()
    {
        len = 0;
        capacity = 0;
        if (data != nullptr) KernelFree(data);
        data = nullptr;
    }

    void EraseValue(T val)
    {
        for(size_t i = 0; i < len; i++)
        {
            if (At(i) == val)
            {
                Erase(i);
                break;
            }
        }
    }

    T* Data()
    {
        return data;
    }

    //allow for direct access to the data using the [] operator
    T& operator[](size_t index)
    {
        return data[index];
    }
private:
    size_t len;
    size_t capacity;
    T* data;
};