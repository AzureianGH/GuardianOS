#pragma once
/// @brief Get the RGB value
/// @param r Red
/// @param g Green
/// @param b Blue
/// @return The RGB value as a bitshifted integer
int rgb(int r, int g, int b);
/// @brief Get the RGBA value
/// @param r Red
/// @param g Green
/// @param b Blue
/// @param a Alpha
/// @return The RGBA value as a bitshifted integer
long rgba(int r, int g, int b, int a);
/// @brief Get the RGB value from an RGBA value
/// @param color The RGBA value
/// @return The RGB value as a bitshifted integer
int a2rgb(long color);