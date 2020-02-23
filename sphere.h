#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "hitable.h"

typedef struct sphere Sphere;

struct sphere {
    Vector center; 
    float radius;
    struct {
        void *inst;
        bool (*scatter)(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered);
    } mat_ptr;
    //struct material mat;

    Hitable hitable;
    //HITABLE_MEMBER(Sphere);
};

Sphere new_sphere(
    Vector v, float r, void *m,
    bool (*scatter)(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered)
);

#endif /* _SPHERE_H_ */
