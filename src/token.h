#ifndef _TOKEN_H
#define _TOKEN_H
#include <string.h>
#include <ctype.h>

#include "stack.h"

extern void init();

typedef struct _token_struct {
    char* value;
    enum {
        //symbols
        T_LPAR,
        T_RPAR,
        T_LBRACK,
        T_RBRACK,
        T_COMMA,
        T_PLUS,
        T_MINUS,
        T_MUL,
        T_DIV,
        T_POW,
        T_MOD,
        T_EQUAL,
        T_COLON,
        // keywords
        T_FUN,
        T_RETURN,
        T_PASS,
        T_FOR,
        T_IN,
        T_TO,
        T_DOWNTO,
        T_STEP,
        T_BREAK,
        // data
        T_NUMERIC,
        T_CHAR,
        T_STRING,
        // misc
        T_TAB,
        T_ID,
        T_EOL,
        T_EOF
    } type;
} token;

token* newToken(char* value, short type);

token* lexstr(char* src, char* cc, unsigned int* index);

token* lexdgt(char* src, char* cc, unsigned int* index);

token* lexstrliteral(char* src, char* cc, unsigned int* index, char delimiter);

token* nextToken(char* src, char* cc, unsigned int* index);
#endif