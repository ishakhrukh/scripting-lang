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
    vv->data = realloc(vv->data, vv->size * vv->_elem_size);
    vv->data[vv->size - 1] = data;
}

void vvpop(voidvector* vv, int index) {
    vv->data[index] = vv->data[vv->size - 1];
    vv->size -= 1;
    vv->data = realloc(vv->data, vv->size);
}