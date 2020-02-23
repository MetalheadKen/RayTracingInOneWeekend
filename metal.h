#ifndef _METAL_H_
#define _METAL_H_

#include "material.h"

typedef struct metal Metal;

struct metal {
    Vector albedo; /* Store attenuation coefficient */
    float fuzz;

    Material mat; /* scatter function get reflected light and attenuation coefficient */
};

Metal new_metal(Vector v, float f);
Vector reflect(const Vector v, const Vector n);

#endif /* _METAL_H_ */
