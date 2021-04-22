#ifndef _INTERPRETER_H
#define _INTERPRETER_H
#include <stdio.h>
#include "tree.h"
#include "parse.h"
#include "stack.h"

typedef struct _function_struct {
    voidvector* local_stack;
} function_data;

extern void init();

void getVal(AST* variable, voidvector* stack);

void interpretAssignment(AST* ast, void* _struct, short flags);

void interpretCall(AST* function);

void interpret(AST* ast, void* _struct, short flags);
#endif