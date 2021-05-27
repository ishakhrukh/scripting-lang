#ifndef _STACK_H
#define _STACK_H
#include <stdlib.h>
#include <stdio.h>

typedef struct _void_vector_struct {
    int size, _elem_size;
    void** data;
} voidvector;

typedef struct _stack_data_struct {
    short type;
    
    // normal variable
    char* id, * value;

    // list
    voidvector* elements;
} stack_data;

voidvector* newvoidvector(int dataSize);

void vvadd(voidvector* vv, void* data);

void vvpop(voidvector* vv, int index);

voidvector* stack, * function_stack;
char* _last_return;
int _last_return_datatype;
FILE* _inputbuf;
#endif