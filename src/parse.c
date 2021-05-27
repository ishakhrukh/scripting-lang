#include "parse.h"

AST* parse(parser* ps) {
    AST* ast;
    switch (ps->tok->type) {
    case T_ID:
        return parseId(ps);
    case T_FUN:
        return parseFunction(ps);
    case T_FOR:
        return parseForLoop(ps);
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
        TYPECHECK(ast->value->type, "invalid syntax");
        ast->flag = ast->value->type;
        return ast;
    case T_EOL:
        ast = 0;
        return ast;
    default:
        fprintf(stderr, "\033[1;31mError:\033[0m Unexpected token of type %d.\n", ps->tok->type);
        init();
    }
    return 0;
}

token* expect(parser* ps, short type) {
    if (ps->tok->type != type) {
        fprintf(stderr, "\033[1;31mError:\033[0m Unexpected token '%s', was expecting token of type %d.\n", ps->tok->value, type);
        init();
    }
    ps->tok = nextToken(ps->src, &ps->cc, &ps->index);
    return ps->tok;
}

AST* parseId(parser* ps) {
    AST* variable, * list_elem;
    char* name;
    int i;
    name = malloc(strlen(ps->tok->value) + 1);
    strcpy(name, ps->tok->value);
    expect(ps, T_ID);
    if (ps->tok->type == T_EQUAL) {  // parse assignment
        variable = newAST(A_ASSIGNMENT);
        variable->name = malloc(strlen(name));
        strcpy(variable->name, name);
        expect(ps, T_EQUAL);
        variable->type = A_ASSIGNMENT;
        if (ps->tok->type == T_LBRACK) {  // parse list assignment (eg. `hi = [10, 'some str', 98, 3`)
            expect(ps, T_LBRACK);
            variable->value = newAST(A_LIST);
            list_elem = parse(ps);
            TYPECHECK(list_elem->type, "Bad initializer for list/array element.")
            vvadd(variable->value->children, list_elem);
            while (ps->tok->type == T_COMMA) {
                expect(ps, T_COMMA);
                list_elem = parse(ps);
                TYPECHECK(list_elem->type, "Bad initializer for list/array element.")
                vvadd(variable->value->children, list_elem);
            }
            expect(ps, T_RBRACK);
        }
        else  // parse regular assignment (eg. `hi = 9`, `var = anothervar`, `response = _nextLine('enter response: ')`)
            variable->value = parse(ps);
        return variable;
    }
    else if (ps->tok->type == T_LPAR) {  // parse call (eg. `hello()`, `_write(1, 'hello world\n')`)
        variable = newAST(A_CALL);
        variable->name = malloc(strlen(name));
        strcpy(variable->name, name);
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
    variable = newAST(A_VARIABLE);
    variable->name = malloc(strlen(name));
    strcpy(variable->name, name);
    return variable;
}

AST* parseFunction(parser* ps) {
    AST* function = newAST(A_FUNCTION), * param;
    parser* _ps = malloc(sizeof(parser));
    // buffer for STDIN
    char STDIN_buf[256];
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
        // file reading
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

AST* parseForLoop(parser* ps) {
    AST* loop = newAST(A_FORLOOP), * iterator = newAST(A_ASSIGNMENT), * target, * step;
    parser* _ps = malloc(sizeof(parser));
    char buf[256];
    expect(ps, T_FOR);
    iterator->name = malloc(strlen(ps->tok->value));
    strcpy(iterator->name, ps->tok->value);
    expect(ps, T_ID);
    if (ps->tok->type == T_EQUAL)
        expect(ps, T_EQUAL);
    else if (ps->tok->type == T_IN) {  // parse for each loop (eg. `for i in iterable_element:`)
        loop->type = A_FOREACHLOOP;
        expect(ps, T_IN);
        loop->value = parse(ps);
        if (loop->value->type != A_VARIABLE) {
            fprintf(stderr, "\033[1;31mError:\033[0m invalid syntax\n");
            init();
        }
        expect(ps, T_COLON);
        return loop;
    }
    else {
        fprintf(stderr, "\033[1;31mError:\033[0m Unexpected token '%s', was expecting an assignment or 'in'\n", ps->tok->value);
        init();
    }
    iterator->value = parse(ps);
    vvadd(loop->children, iterator);
    if (ps->tok->type == T_TO) {
        expect(ps, T_TO);
        loop->flag = LOOP_TO;
    }
    else if (ps->tok->type == T_DOWNTO) {
        expect(ps, T_DOWNTO);
        loop->flag = LOOP_DOWNTO;
    }
    else {
        fprintf(stderr, "\033[1;31mError:\033[0m Unexpected token '%s', was expecting 'to' or 'downto'.\n", ps->tok->value);
        init();
    }
    target = parse(ps);
    if (target->type != A_INTEGER && target->type != A_VARIABLE && target->type != A_CALL) {
        fprintf(stderr, "\033[1;31mError:\033[0m Invalid condition for loop. code = %d\n", target->type);
        init();
    }
    vvadd(loop->children, target);
    if (ps->tok->type == T_STEP) {
        expect(ps, T_STEP);
        step = parse(ps);
        if (step->type != A_INTEGER && step->type != A_VARIABLE && step->type != A_CALL) {
            fprintf(stderr, "\033[1;31mError:\033[0m Invalid condition for loop. code = %d\n", target->type);
            init();
        }
        vvadd(loop->children, step);
    }
    else
        vvadd(loop->children, 0);
    expect(ps, T_COLON);
    for (;;) {
        printf("... ");
        fgets(buf, 256, _inputbuf);
        if (buf[0] != 32 && buf[1] != 32 && buf[2] != 32 && buf[3] != 32) {
            if (buf[0] != '\t')
                break;
        }
        _ps->src = buf;
         _ps->index = 1;
        _ps->cc = buf[1];
        _ps->tok = nextToken(_ps->src, &_ps->cc, &_ps->index);
        if (ps->tok->type == T_FUN) {
            fprintf(stderr, "\033[1;31mError:\033[0m invalid syntax\n");
            init();
        }
        vvadd(loop->children, parse(_ps));
    }
    return loop;
}