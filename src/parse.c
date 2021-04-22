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
        parserConsume(ps, T_LPAR);
        if (ps->tok->type == T_NUMERIC || ps->tok->type == T_STRING) {
            variable->children = newvoidvector(sizeof(AST*));
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
    for (i = 0; i < stack->size; ++i) {
        temp = stack->data[i];
        if (strcmp(temp->id, variable->name) == 0) {
            variable->cval = malloc(strlen(temp->value) * sizeof(char));
            strcpy(variable->cval, temp->value);
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
    if (ps->tok->type == T_LPAR) {
        parserConsume(ps, T_LPAR);
        if (ps->tok->type == T_ID) {
            args = newAST(A_PARAM);
            args->name = ps->tok->value;
            vvadd(function->children, args);
            parserConsume(ps, T_ID);
            function->argcount += 1;
            while (ps->tok->type == T_COMMA) {
                parserConsume(ps, T_COMMA);
                args = newAST(A_PARAM);
                args->name = ps->tok->value;
                vvadd(function->children, args);
                parserConsume(ps, T_ID);
                function->argcount += 1;
            }
        }
        parserConsume(ps, T_RPAR);
    }
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
    case T_RETURN:
        ast = newAST(A_RETURN);
        parserConsume(ps, T_RETURN);
        ast->value = parseExpr(ps);
        return ast;
    case T_EOL:
        ast = 0;
        return ast;
    default:
        printf("Error: Unexpected token of type `%d'\n", ps->tok->type);
        init();
    }
    return 0;
}