#ifndef _DIELECTRIC_H_
#define _DIELECTRIC_H_

#include "material.h"
#include "metal.h"

typedef struct dielectric Dielectric;

struct dielectric {
    float ref_idx;

    Material mat; /* scatter function get reflected light and attenuation coefficient */
};

Dielectric new_dielectric(float ri);

#endif /* _DIELECTRIC_H_ */
