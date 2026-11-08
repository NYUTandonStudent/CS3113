#include "cs3113.h"

Color ColorFromHex(const char *hex)
{
    // Skip leading '#', if present
    if (hex[0] == '#') hex++;

    // Default alpha = 255 (opaque)
    unsigned int r = 0,
                 g = 0,
                 b = 0,
                 a = 255;

    // 6‑digit form: RRGGBB
    if (sscanf(hex, "%02x%02x%02x", &r, &g, &b) == 3)
    {
        return Color{ static_cast<unsigned char>(r),
                      static_cast<unsigned char>(g),
                      static_cast<unsigned char>(b),
                      static_cast<unsigned char>(a) };
    }

    // 8‑digit form: RRGGBBAA
    if (sscanf(hex, "%02x%02x%02x%02x", &r, &g, &b, &a) == 4)
    {
        return Color{ static_cast<unsigned char>(r),
                      static_cast<unsigned char>(g),
                      static_cast<unsigned char>(b),
                      static_cast<unsigned char>(a) };
    }

    // Fallback – return white so you notice something went wrong
    return RAYWHITE;
}

/**
 * @brief Smoothly adjusts the camera's target position towards a specified
 * target position.
 *
 * @param camera A pointer to a `Camera2D` struct.
 * @param targetPosition A pointer to the position the camera should move
 * towards.
 */
void panCamera(Camera2D *camera, const Vector2 *targetPosition)
{
    Vector2 positionDifference = Vector2Subtract(
        *targetPosition, camera->target
    );

    camera->target = Vector2Add(
        camera->target, Vector2Scale(positionDifference, 0.1f)
    ); // 0.1 = smoothing factor
}