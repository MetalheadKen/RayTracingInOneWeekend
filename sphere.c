#include <math.h>
#include "sphere.h"

static bool _hit(const void *t, const Ray r, float t_min, float t_max, Hit_Record *rec)
{
    const Sphere s = *(const Sphere *) t;

    Vector oc = new_vector(new_point(0, 0, 0));
    
    oc = oc.sub(r.origin(r), s.center);

    float a = oc.dot(r.direction(r), r.direction(r));
    float b = oc.dot(oc, r.direction(r));
    float c = oc.dot(oc, oc) - s.radius * s.radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;

        if (temp < t_max && temp > t_min) {
            rec->t = temp;
            rec->p = r.point_at_parameter(r, rec->t);
            rec->normal = rec->p.div_scalar(rec->p.sub(rec->p, s.center), s.radius);
            rec->mat_ptr.inst = s.mat_ptr.inst;
            rec->mat_ptr.scatter = s.mat_ptr.scatter;

            return true;
        }

        temp = (-b + sqrt(discriminant)) / a;

        if (temp < t_max && temp > t_min) {
            rec->t = temp;
            rec->p = r.point_at_parameter(r, rec->t);
            rec->normal = rec->p.div_scalar(rec->p.sub(rec->p, s.center), s.radius);
            rec->mat_ptr.inst = s.mat_ptr.inst;
            rec->mat_ptr.scatter = s.mat_ptr.scatter;

            return true;
        }
    }

    return false;
}

Sphere new_sphere(Vector v, float r, void *m, bool (*scatter)(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered))
{
    return (Sphere) {
        .center = v,
        .radius = r,
        .mat_ptr.inst = m,
        .mat_ptr.scatter = scatter,

        .hitable = (Hitable) {
            .hit = _hit
        },
    };
}
