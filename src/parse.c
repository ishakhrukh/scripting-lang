#include "parse.h"

token* parserConsume(parser* ps, short type) {
    if (ps->tok->type != type) {
        printf("Unexpected token `%s', was expecting token of type %d\n", ps->tok->value, type);
        init();
    }
    ps->tok = nextToken(ps->src, &ps->cc, &ps->index);
    return ps->tok;
}

AST* parseId(parser* ps) {
    AST* variable = newAST(A_VARIABLE);
    stack_data* temp;
    int i;
    variable->name = malloc((strlen(ps->tok->value) + 1) * sizeof(char));
    strcpy(variable->name, ps->tok->value);
    parserConsume(ps, T_ID);
    if (ps->tok->type == T_EQUAL) {
        parserConsume(ps, T_EQUAL);
        variable->type = A_ASSIGNMENT;
        variable->value = parseExpr(ps);
        return variable;
    }
    if (ps->tok->type == T_LPAR) {
        variable->type = A_CALL;
        variable->argcount = 0;
        parserConsume(ps, T_LPAR);
        if (ps->tok->type == T_ID || ps->tok->type == T_NUMERIC || ps->tok->type == T_STRING) {
            vvadd(variable->children, parseExpr(ps));
            variable->argcount += 1;
            while (ps->tok->type == T_COMMA) {
                parserConsume(ps, T_COMMA);
                vvadd(variable->children, parseExpr(ps));
                variable->argcount += 1;
            }
        }
        parserConsume(ps, T_RPAR);
        return variable;
    }
    if (ps->tok->type == T_EOL) {
        for (i = 0; i < stack->size; ++i) {
            temp = stack->data[i];
            if (strcmp(temp->id, variable->name) == 0) {
                printf("%s\n", temp->value);
            }
        }
        init();
    }
    for (i = 0; i < stack->size; ++i) {
        temp = stack->data[i];
        if (strcmp(temp->id, variable->name) == 0) {
            printf("found match\n");
        }
    }
    return variable;
}

AST* parseMathExpr(parser* ps) {
    //////////////////////////////////////////////////
    // TODO: implement math expressions             //
    // IN PROGRESS                                  //
    //////////////////////////////////////////////////
    return 0;
}

AST* parseFunction(parser* ps) {
    AST* function = newAST(A_FUNCTION), * args;
    parser* _ps = malloc(sizeof(parser));
    char buffer[256];
    parserConsume(ps, T_FUN);
    function->name = malloc((strlen(ps->tok->value) + 1) * sizeof(char));
    strcpy(function->name, ps->tok->value);
    parserConsume(ps, T_ID);
    function->argcount = 0;
    parserConsume(ps, T_LPAR);
    if (ps->tok->type == T_ID) {
        args = newAST(A_ARG);
        args->cval = malloc(strlen(ps->tok->value) * sizeof(char));
        strcpy(args->cval, ps->tok->value);
        vvadd(function->children, args);
        parserConsume(ps, T_ID);
        function->argcount += 1;
        while (ps->tok->type == T_COMMA) {
            parserConsume(ps, T_COMMA);
            args = newAST(A_ARG);
            args->cval = malloc(strlen(ps->tok->value) * sizeof(char));
            strcpy(args->cval, ps->tok->value);
            vvadd(function->children, args);
            parserConsume(ps, T_ID);
            function->argcount += 1;
        }
    }
    parserConsume(ps, T_RPAR);
    //////////////////////////////////////////////////
    // TODO: implement args                         //
    // IN PROGRESS                                  //
    //////////////////////////////////////////////////
    parserConsume(ps, T_EQUAL);
    for (;;) {
        printf("... ");
        fgets(buffer, 256, stdin);
        if (buffer[0] != '\t')
            break;
        _ps->src = buffer;
        _ps->index = 1;
        _ps->cc = buffer[1];
        _ps->tok = nextToken(_ps->src, &_ps->cc, &_ps->index);
        vvadd(function->children, parseExpr(_ps));
    }
    return function;
}

// TEMP FUNCTION:
AST* parsePass(parser* ps) {
    AST* str = newAST(A_NULL);
    str->cval = malloc(4 * sizeof(char));
    strcpy(str->cval, "pass");
    parserConsume(ps, T_PASS);
    return str;
}

AST* parseExpr(parser* ps) {
    AST* ast;
    switch (ps->tok->type) {
    case T_ID:
        return parseId(ps);
    case T_FUN:
        return parseFunction(ps);
    case T_NUMERIC:
        if (strchr(ps->tok->value, '.')) {
            ast = newAST(A_FLOAT);
        }
        else {
            ast = newAST(A_INTEGER);
        }
        ast->cval = malloc((strlen(ps->tok->value) + 1) * sizeof(char));
        strcpy(ast->cval, ps->tok->value);
        parserConsume(ps, T_NUMERIC);
        return ast;
    case T_STRING:
        ast = newAST(A_STRING);
        ast->cval = malloc((strlen(ps->tok->value) + 1) * sizeof(char));
        strcpy(ast->cval, ps->tok->value);
        parserConsume(ps, T_STRING);
        return ast;
    case T_PASS:
        return parsePass(ps);
    default:
        printf("Error: type %d not supported yet\n", ps->tok->type);
        init();
    }
    return 0;
}

AST* parseCompound(parser* base) {
    AST* compound = newAST(A_COMPOUND);
    while (base->tok->type != T_EOL)
        vvadd(compound->children, parseExpr(base));
    return compound;
}