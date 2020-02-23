#include <stdlib.h>
#include <math.h>
#include "dielectric.h"

static bool _scatter(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered)
{
    const Dielectric diel = *(const Dielectric *) t;

    /* Get diffuse reflection light and attenuation coefficient of material */
    Vector outward_normal;
    Vector reflected = reflect(r_in.direction(r_in), rec.normal);

    float ni_over_nt;
    *attenuation = new_vector(new_point(1.0, 1.0, 1.0));

    Vector refracted = new_vector(new_point(0, 0, 0));
    float reflect_prob;
    float cosine;

    if (refracted.dot(r_in.direction(r_in), rec.normal) > 0) {
        // outward_normal = -rec.normal
        outward_normal = refracted.mul_scalar(rec.normal, -1.0);
        ni_over_nt = diel.ref_idx;

        Vector v = r_in.direction(r_in);

        cosine = refracted.dot(v, rec.normal) / v.length(v);
        cosine = sqrt(1 - diel.ref_idx * diel.ref_idx * (1 - cosine * cosine));
    } else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / diel.ref_idx;

        Vector v = r_in.direction(r_in);

        cosine = refracted.dot(v, rec.normal) * -1.0 / v.length(v);
    }

    if (refract(r_in.direction(r_in), outward_normal, ni_over_nt, &refracted)) {
        reflect_prob = schlick(cosine, diel.ref_idx);
    } else {
        reflect_prob = 1.0;
    }

    if (drand48() < reflect_prob) {
        *scattered = new_ray(rec.p, reflected);
    } else {
        *scattered = new_ray(rec.p, refracted);
    }

    return true;
}

Dielectric new_dielectric(float ri)
{
    return (Dielectric) {
        .ref_idx = ri,

        .mat = (Material) {
            .scatter = _scatter
        },
    };
}
