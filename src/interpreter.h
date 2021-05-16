#ifndef _INTERPRETER_H
#define _INTERPRETER_H
#include "tree.h"
#include "parse.h"
#include "stack.h"
#include "builtins.h"

#define returns(value) \
    free(_last_return); \
    _last_return = malloc(strlen(value)); \
    strcpy(_last_return, value)

typedef struct _function_struct {
    voidvector* local_stack;
} function_data;

extern void init();

void getVal(AST* variable, voidvector* stack);

void interpretAssignment(AST* ast, void* _struct);

void interpretCall(AST* function, voidvector* ps);

void interpret(AST* ast, void* _struct);
#endif