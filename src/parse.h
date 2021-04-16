#ifndef _PARSE_H
#define _PARSE_H
#include <stdio.h>
#include <ctype.h>
#include "tree.h"
#include "token.h"

typedef struct _parser_struct {
    char* src, cc;
    unsigned int index;
    token* tok;
} parser;

token* parserConsume(parser* ps, short type);

AST* parseId(parser* ps);

AST* parseMathExpr(parser* ps);

AST* parseFunction(parser* ps);

AST* parseExpr(parser* ps);

AST* parseCompound(parser* base);
#endif