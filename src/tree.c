#include "tree.h"
#include "stack.h"

AST* newAST(int type) {
    AST* a = malloc(sizeof(AST));
    a->type = type;
    if (type == A_FUNCTION)
        a->children = newvoidvector(sizeof(AST*));
    if (type == A_FUNCTION || type == A_CALL)
        a->argcount = 0;
    return a;
}