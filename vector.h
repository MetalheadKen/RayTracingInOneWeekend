#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "primitive.h"

typedef struct vector Vector;

struct vector {
    Point point;
    Color color;

    Vector (*add)(Vector a, Vector b);
    Vector (*sub)(Vector a, Vector b);
    Vector (*mul)(Vector a, Vector b);
    Vector (*div)(Vector a, Vector b);

    Vector (*mul_scalar)(Vector a, float val);
    Vector (*div_scalar)(Vector a, float val);

    float (*dot)(Vector a, Vector b);
    Vector (*cross)(Vector a, Vector b);

    float (*length)(Vector v);
    float (*squared_length)(Vector v);
    Vector (*make_unit)(Vector *v);
};

Vector new_vector(Point p);

static inline Vector unit_vector(Vector v)
{
    return v.div_scalar(v, v.length(v));
}

#endif /* _VECTOR_H_ */
