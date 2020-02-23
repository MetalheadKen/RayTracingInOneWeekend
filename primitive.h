#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include <stdint.h>

typedef struct point Point;
typedef struct color Color;

struct point {
    float x, y, z;
};

struct color {
    float r, g, b;
};

static inline Point new_point(float x, float y, float z)
{
    return (Point) { x, y, z };
}

static inline Color new_color(int r, int g, int b)
{
    return (Color) { r, g, b };
}

#endif /* _PRIMITIVE_H_ */
