#ifndef _PARSE_H
#define _PARSE_H
#include <ctype.h>
#include "tree.h"
#include "token.h"
#include "stack.h"

#define TYPECHECK(type, onError) \
    if (type != A_CALL && type != A_INTEGER && \
    type != A_FLOAT && type != A_STRING && \
    type != A_VARIABLE  \
    ) { \
        fprintf(stderr, "\033[1;31mError:\033[0m %s\n", onError); \
        init(); \
    }

typedef struct _parser_struct {
    char* src, cc;
    unsigned int index;
    token* tok;
} parser;

extern size_t _getline(char** lineptr, size_t* __n, FILE* stream);

AST* parse(parser* ps);

token* expect(parser* ps, short type);

AST* parseId(parser* ps);

AST* parseFunction(parser* ps);

AST* parseReturn(parser* ps);

AST* parseForLoop(parser* ps);
#endif