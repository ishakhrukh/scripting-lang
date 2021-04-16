#include "tree.h"
#include "stack.h"

AST* newAST(int type) {
    AST* a = malloc(sizeof(AST));
    a->type = type;
    a->children = newvoidvector(sizeof(AST*));
    return a;
}