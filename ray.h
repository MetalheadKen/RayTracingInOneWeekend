#ifndef _RAY_H_
#define _RAY_H_

#include "vector.h"

typedef struct ray Ray;

struct ray {
    Vector vA, vB; 

    Vector (*origin)(Ray r);
    Vector (*direction)(Ray r);
    Vector (*point_at_parameter)(Ray r, float val);
};

Ray new_ray(Vector a, Vector b);

#endif /* _RAY_H_ */
