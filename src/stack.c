#include "stack.h"

voidvector* newvoidvector(int dataSize) {
    voidvector* vv = malloc(sizeof(voidvector));
    vv->size = 0;
    vv->_elem_size = dataSize;
    vv->data = 0;
    return vv;
}

void vvadd(voidvector* vv, void* data) {
    vv->size += 1;
    if (vv->data == 0)
        vv->data = malloc(vv->_elem_size);
    else
        vv->data = realloc(vv->data, vv->size * vv->_elem_size);
    vv->data[vv->size - 1] = data;
}