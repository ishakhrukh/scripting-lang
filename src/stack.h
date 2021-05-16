#ifndef _STACK_H
#define _STACK_H
#include <stdlib.h>
#include <stdio.h>

typedef struct _void_vector_struct {
    int size, _elem_size;
    void** data;
} voidvector;

typedef struct _stack_data_struct {
    char* id, * value;
    short type;
} stack_data;

voidvector* newvoidvector(int dataSize);

void vvadd(voidvector* vv, void* data);

voidvector* stack, * function_stack;
char* _last_return;
FILE* _inputbuf;
#endif