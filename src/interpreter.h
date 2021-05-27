#ifndef _INTERPRETER_H
#define _INTERPRETER_H
#include "tree.h"
#include "parse.h"
#include "stack.h"
#include "builtins.h"

#define RETURNS(value, type) \
    free(_last_return); \
    _last_return = malloc(strlen(value)); \
    strcpy(_last_return, value); \
    _last_return_datatype = type; \

typedef struct _function_struct {
    voidvector* local_stack;
} function_data;

extern void init();

void interpret(AST* ast, void* _struct);

void getVal(AST* variable, voidvector* stack);

void interpretAssignment(AST* ast, voidvector* ps);

void interpretCall(AST* function, voidvector* ps);

void interpretForLoop(AST* loop, voidvector* ps);

char* _nsum(char* str, char* add);

int _ncmp(char* s1, char* s2);

void chval(char* vname, char* new, voidvector* ps);

int a2i(char* s);
#endif