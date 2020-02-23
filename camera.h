#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"

typedef struct camera Camera;

struct camera {
    Vector origin;
    Vector lower_left_corner;
    Vector horizontal;
    Vector vertical;
    Vector u, v, w;
    float lens_radius;

    Ray (*get_ray)(Camera c, float s, float t);
};

// vfov is top to bottom in degrees
Camera new_camera(Vector lookfrom, Vector lookat, Vector vup, float vfov, float aspect, float aperture, float focus_dist);

#endif /* _CAMERA_H_ */
