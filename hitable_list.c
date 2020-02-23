#include "hitable_list.h"

static bool _hit(const void *t, const Ray r, float t_min, float t_max, Hit_Record *rec)
{
    const Hitable_List h = *(const Hitable_List *) t;

    Hit_Record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (int i = 0; i < h.list_size; i++) {
        if (h.list[i]->hit((const void *) h.list[i]->inst, r, t_min, closest_so_far, &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}

Hitable_List new_hitable_list(Hitable **h, int size)
{
    return (Hitable_List) {
        .list = h,
        .list_size = size,

        .hitable = (Hitable) {
            .hit = _hit
        }
    };
}
