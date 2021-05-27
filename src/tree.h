#ifndef _AST_H
#define _AST_H
#include "stack.h"

#define LOOP_TO 1
#define LOOP_DOWNTO 2

typedef struct _abstract_syntax_tree {
    short datatype, argcount, flag;
    char* name, * cval;  // 'cval' is short for 'charpointerval' (basically string value), which will contain the value of a variable
    voidvector* children;
    struct _abstract_syntax_tree* value;
    enum {
        A_CALL,
        A_ASSIGNMENT,
        A_FUNCTION,
        A_PARAM,
        A_FORLOOP,
        A_FOREACHLOOP,
        // STATEMENTS
        A_RETURN,
        A_BREAK,
        // DATA TYPE
        A_INTEGER,
        A_FLOAT,
        A_STRING,
        A_LIST,
        // MISC
        A_LIST_ELEMENT,
        A_VARIABLE
    } type;
} AST;

AST* newAST(short type);
#endif