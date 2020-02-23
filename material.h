#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "hitable.h"

#define MATERIAL_MEMBER(TYPE) \
    bool (*scatter)(const TYPE, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered)

typedef struct material Material;

struct material {
    //MATERIAL_MEMBER(Material);

    bool (*scatter)(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered);
};

Vector reflect(const Vector v, const Vector n);
bool refract(const Vector v, const Vector n, float ni_over_nt, Vector *refracted);
float schlick(float cosine, float ref_idx);
Vector random_in_uint_sphere(void);

#endif /* _MATERIAL_H_ */
