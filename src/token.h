#ifndef _TOKEN_H
#define _TOKEN_H
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

extern void init();

typedef struct _token_struct {
    char* value;
    enum {
        //symbols
        T_LPAR,
        T_RPAR,
        T_COMMA,
        T_PLUS,
        T_MINUS,
        T_MUL,
        T_DIV,
        T_POW,
        T_MOD,
        T_EQUAL,
        // keywords 9
        T_FUN,
        T_RETURN,
        T_PASS,
        // data 12
        T_NUMERIC,
        T_CHAR,
        T_STRING,
        // misc 15
        T_TAB,
        T_ID,
        T_EOL
    } type;
} token;

token* newToken(char* value, short type);

token* lexstr(char* src, char* cc, unsigned int* index);

token* lexdgt(char* src, char* cc, unsigned int* index);

token* lexstrliteral(char* src, char* cc, unsigned int* index, char delimiter);

token* nextToken(char* src, char* cc, unsigned int* index);
#endif