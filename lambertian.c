#include <stdlib.h>
#include "lambertian.h"

static bool _scatter(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered)
{
    const Lambertian lam = *(const Lambertian *) t;

    /* Get diffuse reflection light and attenuation coefficient of material */
    Vector target = new_vector(new_point(0, 0, 0));

    target = target.add(rec.p, rec.normal);
    target = target.add(target, random_in_uint_sphere());

    *scattered = new_ray(rec.p, target.sub(target, rec.p));
    *attenuation = lam.albedo;

    return true;
}

Lambertian new_lambertian(Vector v)
{
    return (Lambertian) {
        .albedo = v,

        .mat = (Material) {
            .scatter = _scatter
        },
    };
}
