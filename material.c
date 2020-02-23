#include <stdlib.h>
#include <math.h>
#include "material.h"

Vector reflect(const Vector v, const Vector n)
{
    Vector vp = new_vector(new_point(0, 0, 0));

    float tmp = 2.0 * v.dot(v, n);
    vp = vp.mul_scalar(n, tmp);
    vp = vp.sub(v, vp);

    return vp;
}

bool refract(const Vector v, const Vector n, float ni_over_nt, Vector *refracted)
{
    Vector uv = unit_vector(v);
    float dt = uv.dot(uv, n);
    float discriminat = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);

    if (discriminat > 0) {
        // ni_over_nt * (uv - n * dt) - n * sqrt(discriminat)
        Vector tmp = uv.sub(uv, n.mul_scalar(n, dt));
        tmp = uv.mul_scalar(tmp, ni_over_nt);
        tmp = tmp.sub(tmp, n.mul_scalar(n, sqrt(discriminat)));
        *refracted = tmp;

        return true;
    } else {
        return false;
    }
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

Vector random_in_uint_sphere(void)
{
    Vector p;
    
    do {
        p = new_vector(new_point(drand48(), drand48(), drand48()));
        p = p.mul_scalar(p, 2.0);
        p = p.sub(p, new_vector(new_point(1, 1, 1)));
    } while (p.squared_length(p) >= 1.0);

    return p;
}

