#ifndef RGB_H

#define RGB_H

#include "base_classes.h"

inline uint32_t to_RGB(Vector3 color)
{
	return
		(((uint32_t)(color.X() * 255)) & 0xFF) |
		((((uint32_t)(color.Y() * 255)) & 0xFF) << 8) |
		((((uint32_t)(color.Z() * 255)) & 0xFF) << 16);
}

inline Vector3 from_RGB(uint32_t color)
{
	return Vector3(
		((double)(color & 0xFF)) / 255,
		((double)((color >> 8) & 0xFF)) / 255,
		((double)((color >> 16) & 0xFF)) / 255);
}

#endif
