#include "tree.h"
#include "stack.h"

AST* newAST(short type) {
    AST* a = malloc(sizeof(AST));
    a->type = type;
    a->value = 0;
    if (type == A_FUNCTION || type == A_FORLOOP || type == A_LIST)
        a->children = newvoidvector(sizeof(AST*));
    if (type == A_FUNCTION || type == A_CALL)
        a->argcount = 0;
    return a;
}