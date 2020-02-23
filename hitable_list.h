#ifndef _HITABLE_LIST_H_
#define _HITABLE_LIST_H_ 

#include "hitable.h"

typedef struct hitable_list Hitable_List;

struct hitable_list {
    Hitable **list;
    int list_size;

    Hitable hitable;
    //HITABLE_MEMBER(Hitable_List);
};

Hitable_List new_hitable_list(Hitable **h, int size);

#endif /* _HITABLE_LIST_H_ */
