#ifndef _AST_H
#define _AST_H
#include "stack.h"

typedef struct _abstract_syntax_tree {
    short datatype, argcount;
    char* name, * cval;  // 'cval' is short for 'charpointerval' (basically string value), which will contain the value of a variable
    voidvector* children;
    struct _abstract_syntax_tree* value;
    enum {
        A_CALL,
        A_ASSIGNMENT,
        A_FUNCTION,
        A_PARAM,
        // STATEMENTS
        A_RETURN,
        // DATA TYPE
        A_INTEGER,
        A_FLOAT,
        A_STRING,
        // MISC
        A_VARIABLE
    } type;
} AST;

AST* newAST(short type);
#endif