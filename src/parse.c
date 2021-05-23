#include "parse.h"

AST* parse(parser* ps) {
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
        ast->cval = malloc(strlen(ps->tok->value) + 1);
        strcpy(ast->cval, ps->tok->value);
        expect(ps, T_NUMERIC);
        return ast;
    case T_STRING:
        ast = newAST(A_STRING);
        ast->cval = malloc(strlen(ps->tok->value) + 1);
        strcpy(ast->cval, ps->tok->value);
        expect(ps, T_STRING);
        return ast;
    case T_RETURN:
        ast = newAST(A_RETURN);
        expect(ps, T_RETURN);
        ast->value = parse(ps);
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

token* expect(parser* ps, short type) {
    if (ps->tok->type != type) {
        printf("Unexpected token '%s', was expecting token of type %d\n", ps->tok->value, type);
        init();
    }
    ps->tok = nextToken(ps->src, &ps->cc, &ps->index);
    return ps->tok;
}

AST* parseId(parser* ps) {
    AST* variable = newAST(A_VARIABLE);
    stack_data* temp;
    int i;
    variable->name = malloc(strlen(ps->tok->value) + 1);
    strcpy(variable->name, ps->tok->value);
    expect(ps, T_ID);
    if (ps->tok->type == T_EQUAL) {
        expect(ps, T_EQUAL);
        variable->type = A_ASSIGNMENT;
        variable->value = parse(ps);
        return variable;
    }
    if (ps->tok->type == T_LPAR) {
        variable->type = A_CALL;
        expect(ps, T_LPAR);
        if (ps->tok->type == T_NUMERIC || ps->tok->type == T_STRING || ps->tok->type == T_ID) {
            variable->children = newvoidvector(sizeof(AST*));
            vvadd(variable->children, parse(ps));
            variable->argcount += 1;
            while (ps->tok->type == T_COMMA) {
                expect(ps, T_COMMA);
                vvadd(variable->children, parse(ps));
                variable->argcount += 1;
            }
        }
        expect(ps, T_RPAR);
        return variable;
    }
    variable->type = A_VARIABLE;
    return variable;
}

AST* parseFunction(parser* ps) {
    AST* function = newAST(A_FUNCTION), * param;
    parser* _ps = malloc(sizeof(parser));
    // buffer for STDIN
    char STDIN_buf[256];
    // buffer for file reading
    char* buffer = 0;
    size_t __n = 0;

    expect(ps, T_FUN);
    function->name = malloc(strlen(ps->tok->value) + 1);
    strcpy(function->name, ps->tok->value);
    expect(ps, T_ID);
    function->argcount = 0;
    if (ps->tok->type == T_LPAR) {
        expect(ps, T_LPAR);
        if (ps->tok->type == T_ID) {
            param = newAST(A_PARAM);
            param->name = ps->tok->value;
            vvadd(function->children, param);
            expect(ps, T_ID);
            function->argcount += 1;
            while (ps->tok->type == T_COMMA) {
                expect(ps, T_COMMA);
                param = newAST(A_PARAM);
                param->name = ps->tok->value;
                vvadd(function->children, param);
                expect(ps, T_ID);
                function->argcount += 1;
            }
        }
        expect(ps, T_RPAR);
    }
    expect(ps, T_EQUAL);
    if (_inputbuf != stdin) {
        while (_getline(&buffer, &__n, _inputbuf) != -1) {
            if (buffer[0] == 32 && buffer[1] == 32 && buffer[2] == 32 && buffer[3] == 32) {
                _ps->src = buffer;
                _ps->index = 1;
                _ps->cc = buffer[1];
                _ps->tok = nextToken(_ps->src, &_ps->cc, &_ps->index);
                vvadd(function->children, parse(_ps));
                continue;
            }
            else if (buffer[0] == '\t') {
                _ps->src = buffer;
                _ps->index = 1;
                _ps->cc = buffer[1];
                _ps->tok = nextToken(_ps->src, &_ps->cc, &_ps->index);
                vvadd(function->children, parse(_ps));
                continue;
            }
            break;
        }
    }
    else {
        for (;;) {
            printf("... ");
            fgets(STDIN_buf, 256, _inputbuf);
            if (STDIN_buf[0] != 32 && STDIN_buf[1] != 32) {
                if (STDIN_buf[0] != '\t')
                    break;
            }
            _ps->src = STDIN_buf;
            _ps->index = 1;
            _ps->cc = STDIN_buf[1];
            _ps->tok = nextToken(_ps->src, &_ps->cc, &          _ps->index);
            vvadd(function->children, parse(_ps));
        }
    }
    return function;
}