#ifndef _HITABLE_H_
#define _HITABLE_H_

#include <stdbool.h>
#include "ray.h"

#define HITABLE_MEMBER(TYPE) \
    bool (*hit)(const TYPE t, const Ray r, float t_min, float t_max, Hit_Record *rec);

typedef struct hit_record Hit_Record;
typedef struct hitable Hitable;

struct material;

struct hit_record {
    float t;
    Vector p;
    Vector normal;
    struct {
        void *inst;
        bool (*scatter)(const void *t, const Ray r_in, const Hit_Record rec, Vector *attenuation, Ray *scattered);
    } mat_ptr;
};

struct hitable {
    //HITABLE_MEMBER(Hitable);

    void *inst;
    bool (*hit)(const void *t, const Ray r, float t_min, float t_max, Hit_Record *rec);
};

#endif /* _HITABLE_H_ */
