#ifndef _PARSE_H
#define _PARSE_H
#include <ctype.h>
#include "tree.h"
#include "token.h"
#include "stack.h"

typedef struct _parser_struct {
    char* src, cc;
    unsigned int index;
    token* tok;
} parser;

extern size_t _getline(char** lineptr, size_t* __n, FILE* stream);

token* parserConsume(parser* ps, short type);

AST* parseId(parser* ps);

AST* parseMathExpr(parser* ps);

AST* parseFunction(parser* ps);

AST* parseReturn(parser* ps);

AST* parseExpr(parser* ps);
#endif