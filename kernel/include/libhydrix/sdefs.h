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



/**-----------------------------------------------------------------------------

 @file    vector.h
 @brief   vector - fundamental data structure, variable length array
 @details
 @verbatim

  Implementation of variable length array. All the functions are defined in
  macros.

 @endverbatim

 @attention If memories are randomly initialized, random "data" will cause #PF.
 @attention Pls make sure memories are initially set to zero.

 **-----------------------------------------------------------------------------
 */
#pragma once

#include <stdint.h>

#define VECTOR_RESIZE_FACTOR        4

#define vec_struct(type)                                            \
    struct {                                                        \
        size_t len;                                                 \
        size_t capacity;                                            \
        type*  data;                                                \
    }

#define vec_extern(type, name)      extern vec_struct(type) name
#define vec_new(type, name)         vec_struct(type) name = {0}
#define vec_new_static(type, name)  static vec_new(type, name)

#define vec_push_back(vec, elem)                                   \
    {                                                               \
        if ((vec)->len == (vec)->capacity) {                        \
            (vec)->capacity = (vec)->capacity ? (vec)->capacity * VECTOR_RESIZE_FACTOR : 1; \
            (vec)->data = (typeof((vec)->data))KernelReallocate((vec)->data, sizeof((vec)->data[0]) * (vec)->capacity); \
        }                                                           \
        (vec)->data[(vec)->len++] = (elem);                         \
    }

#define vec_length(vec)             (vec)->len
#define vec_at(vec, index)          (vec)->data[index]

#define vec_erase(vec, index)                                       \
    {                                                               \
        memcpy(&((vec)->data[index]), &((vec)->data[index + 1]),    \
               sizeof((vec)->data[0]) * (vec)->len - index - 1);    \
        (vec)->len--;                                               \
    };

#define vec_erase_all(vec)                                          \
    {                                                               \
        (vec)->len = 0;                                             \
        (vec)->capacity = 0;                                        \
        if ((vec)->data != NULL) KernelFree((vec)->data);               \
        (vec)->data = NULL;                                         \
    }

#define vec_erase_val(vec, val)                                     \
    {                                                               \
        for(size_t __i = 0; __i < (vec)->len; __i++) {              \
            if (vec_at(vec, __i) == (val)) {                        \
                vec_erase(vec, __i);                                \
                break;                                              \
            }                                                       \
        }                                                           \
    }

#define nullptr 0