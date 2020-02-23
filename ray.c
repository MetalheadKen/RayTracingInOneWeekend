#include "ray.h"

static Vector _origin(Ray r)
{
    return r.vA;
}

static Vector _direction(Ray r)
{
    return r.vB;
}

static Vector _point_at_parameter(Ray r, float val)
{
    // A + t * B
    Vector vec = new_vector(new_point(0, 0, 0));

    return vec.add(r.vA, vec.mul_scalar(r.vB, val));
}

Ray new_ray(Vector a, Vector b)
{
    return (Ray) {
        .vA = a,
        .vB = b,

        .origin = _origin,
        .direction = _direction,
        .point_at_parameter = _point_at_parameter,
    };
}
