#include "token.h"

token* newToken(char* value, short type) {
    token* t = malloc(sizeof(token));
    t->value = value;
    t->type = type;
    return t;
}

token* lexstr(char* src, char* cc, unsigned int* index) {
    char* value = malloc(sizeof(char));
    while (isalpha(*cc) || *cc == '_') {
        value = realloc(value, strlen(value) + 1);
        strcat(value, (char[]){*cc, 0});
        ++*index;
        *cc = src[*index];
    }
    if (strcmp(value, "fun") == 0)
        return newToken(value, T_FUN);
    if (strcmp(value, "return") == 0)
        return newToken(value, T_RETURN);
    return newToken(value, T_ID);
}

token* lexdgt(char* src, char* cc, unsigned int* index) {
    char* value = malloc(sizeof(char));
    _Bool dot = 0;
    while (isdigit(*cc) || *cc == '.') {
        if (*cc == '.') {
            if (dot == 1) {
                printf("Error: got more than one decimal point on a decimal number\n");
                init();
            }
            ++dot;
        }
        value = realloc(value, (strlen(value) + 1));
        strcat(value, (char[]){*cc, 0});
        ++*index;
        *cc = src[*index];
    }
    return newToken(value, T_NUMERIC);
}

token* lexstrliteral(char* src, char* cc, unsigned int* index, char delimiter) {
    char* value = malloc(sizeof(char)), * formatted = malloc(sizeof(char)), escape_seq;
    int i = 0;
    ++*index;
    *cc = src[*index];
    while (*cc != delimiter) {
        if (*cc == '\\') {
            value = realloc(value, (strlen(value) + 2));
            ++*index;
            *cc = src[*index];
            strcat(value, (char[]){'\\', *cc, 0});
            ++*index;
            *cc = src[*index];
            continue;
        }
        value = realloc(value, (strlen(value) + 1));
        strcat(value, (char[]){*cc, 0});
        ++*index;
        *cc = src[*index];
    }
    ++*index;
    *cc = src[*index];
    while (value[i] != 0 && i < strlen(value)) {
        if (value[i] == '\\') {
            ++i;
            switch (value[i]) {
            case 'n':
                escape_seq = '\n';
                break;
            case 'r':
                escape_seq = '\r';
                break;
            case 't':
                escape_seq = '\t';
                break;
            case '"':
                escape_seq = '\"';
                break;
            default:
                printf("bad escape sequence '%s'\n", (char[]){'\\', *cc, 0});
                init();
            }
            ++i;
            formatted = realloc(formatted, (strlen(formatted) + 1));
            strcat(formatted, (char[]){escape_seq, 0});
        }
        else {
            formatted = realloc(formatted, (strlen(formatted) + 2));
            strcat(formatted, (char[]){value[i], 0});
            ++i;
        }
    }
    free(value);
    return newToken(formatted, T_STRING);
}

token* nextToken(char* src, char* cc, unsigned int* index) {
    while (*cc != '\n') {
        while (*cc == ' ' || *cc == '\t') {
            ++*index;
            *cc = src[*index];
        }
        if (isalpha(*cc) || *cc == '_')
            return lexstr(src, cc, index);
        if (isdigit(*cc))
            return lexdgt(src, cc, index);
        switch (*cc) {
        case '"':
            return lexstrliteral(src, cc, index, '"');
        case '\'':
            return lexstrliteral(src, cc, index, '\'');
        case '\n':
            ++*index;
            *cc = src[*index];
            return newToken("\n", T_EOL);
        case '\t':
            ++*index;
            *cc = src[*index];
            return newToken("\t", T_TAB);
        case '(':
            ++*index;
            *cc = src[*index];
            return newToken("(", T_LPAR);
        case ')':
            ++*index;
            *cc = src[*index];
            return newToken(")", T_RPAR);
        case ',':
            ++*index;
            *cc = src[*index];
            return newToken(",", T_COMMA);
        case '+':
            ++*index;
            *cc = src[*index];
            return newToken("+", T_PLUS);
        case '-':
            ++*index;
            *cc = src[*index];
            return newToken("-", T_MINUS);
        case '*':
            ++*index;
            *cc = src[*index];
            if (*cc == '*') {
                ++*index;
                *cc = src[*index];
                return newToken("**", T_POW);
            }
            return newToken("*", T_EQUAL);
        case '/':
            ++*index;
            *cc = src[*index];
            return newToken("/", T_DIV);
        case '%':
            ++*index;
            *cc = src[*index];
            return newToken("=", T_MOD);
        case '=':
            ++*index;
            *cc = src[*index];
            return newToken("=", T_EQUAL);
        case 0:
            if (_inputbuf != stdin) {
                ++*index;
                *cc = src[*index];
                return newToken(0, T_EOF);
            }
        default:
            printf("Error: Unexpected character '%c',  ascii value: %d\n", *cc, *cc);
            init();
        }
    }
    return newToken("\n", T_EOL);
}