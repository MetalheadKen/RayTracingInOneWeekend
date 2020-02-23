#include <stdlib.h>
#include "metal.h"

static bool _scatter(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered)
{
    const Metal met = *(const Metal *) t;

    /* Get diffuse reflection light and attenuation coefficient of material */
    Vector reflected = reflect(unit_vector(r_in.direction(r_in)), rec.normal);

    *scattered = new_ray(rec.p, reflected.add(reflected, reflected.mul_scalar(random_in_uint_sphere(), met.fuzz)));
    *attenuation = met.albedo;

    return (reflected.dot(scattered->direction(*scattered), rec.normal) > 0);
}

Metal new_metal(Vector v, float f)
{
    int fuzz = (f < 1) ? f : 1;

    return (Metal) {
        .albedo = v,
        .fuzz = fuzz,

        .mat = (Material) {
            .scatter = _scatter
        },
    };
}
