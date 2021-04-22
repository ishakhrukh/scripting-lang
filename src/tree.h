#ifndef _AST_H
#define _AST_H
#include "stack.h"

typedef struct _abstract_syntax_tree {
    short datatype, argcount;
    char* name, * cval;
    voidvector* children;
    struct _abstract_syntax_tree* value, * left, * right;
    enum {
        A_CALL,
        A_ASSIGNMENT,
        A_STATEMENT,
        A_FUNCTION,
        A_ARG,
        A_PARAM,
        A_RETURN,
        // DATA
        A_INTEGER,
        A_FLOAT,
        A_STRING,
        A_VARIABLE,
        // NO TYPE
        A_NULL
    } type;
} AST;

AST* newAST(int type);
#endif