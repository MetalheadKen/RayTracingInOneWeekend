#include <stdlib.h>
#include <math.h>
#include "camera.h"

Vector random_in_unit_disk(void)
{
    Vector p;

    do {
        p = new_vector(new_point(drand48(), drand48(), 0));
        p = p.mul_scalar(p, 2.0);
        p = p.sub(p, new_vector(new_point(1, 1, 0)));
    } while (p.dot(p, p) >= 1.0);

    return p;
}

static Ray _get_ray(Camera c, float s, float t)
{
    Vector rd = c.origin.mul_scalar(random_in_unit_disk(), c.lens_radius);
    // offset = c.u * rd.x + c.v * rd.y
    Vector offset = c.u.mul_scalar(c.u, rd.point.x);
    offset = offset.add(offset, c.v.mul_scalar(c.v, rd.point.y));

    // c.lower_left_corner + s * c.horizontal + t * c.vertical - origin - offset
    Vector b = new_vector(new_point(0, 0, 0));
    b = b.add(c.lower_left_corner, b.mul_scalar(c.horizontal, s));
    b = b.add(b, b.mul_scalar(c.vertical, t));
    b = b.sub(b.sub(b, c.origin), offset);

    Vector a = c.origin.add(c.origin, offset);

    return new_ray(a, b);
}

Camera new_camera(Vector lookfrom, Vector lookat, Vector vup, float vfov, float aspect, float aperture, float focus_dist)
{
    Vector u = new_vector(new_point(0, 0, 0));
    Vector v = new_vector(new_point(0, 0, 0));
    Vector w = new_vector(new_point(0, 0, 0));

    float lens_radius = aperture / 2.0;
    float theta = vfov * M_PI / 180;
    float half_height = tan(theta / 2);
    float half_width = aspect * half_height;

    Vector origin = lookfrom;
    w = unit_vector(w.sub(lookfrom, lookat));
    u = unit_vector(u.cross(vup, w));
    v = v.cross(w, u);

    // lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w
    Vector corner = new_vector(new_point(0, 0, 0));
    corner = corner.sub(origin, u.mul_scalar(u, half_width * focus_dist));
    corner = corner.sub(corner, v.mul_scalar(v, half_height * focus_dist));
    corner = corner.sub(corner, w.mul_scalar(w, focus_dist));

    Vector hori = u.mul_scalar(u, 2 * half_width * focus_dist);
    Vector vert = v.mul_scalar(v, 2 * half_height * focus_dist);

    return (Camera) {
        .origin            = origin,
        .lower_left_corner = corner,
        .horizontal        = hori,
        .vertical          = vert,
        .u                 = u,
        .v                 = v,
        .w                 = w,
        .lens_radius       = lens_radius,


        .get_ray = _get_ray,
    };
}
