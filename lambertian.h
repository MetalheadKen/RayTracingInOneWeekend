#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "material.h"

typedef struct lambertian Lambertian;

struct lambertian {
    Vector albedo; /* Store attenuation coefficient */

    Material mat; /* scatter function get reflected light and attenuation coefficient */
};

Lambertian new_lambertian(Vector v);

#endif /* _LAMBERTIAN_H_ */
