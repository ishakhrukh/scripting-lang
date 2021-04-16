#include "tree.h"
#include "parse.h"
#include "stack.h"
#include "token.h"
#include "interpreter.h"

void exec(char* line) {
    parser* ps = malloc(sizeof(parser));
    AST* base;
#pragma region DEBUG
    stack_data* tempvar;
    AST* tempfunc;
    int i;
    if (strcmp(line, "exit\n") == 0)
        exit(0);
    if (strcmp(line, "$stack\n") == 0) {
        for (i = 0; i < stack->size; ++i) {
            tempvar = stack->data[i];
            printf("[%s: %s]\n", tempvar->id, tempvar->value);
        }
        init();
    }
    if (strcmp(line, "$fstack\n") == 0) {
        for (i = 0; i < function_stack->size; ++i) {
            tempfunc = function_stack->data[i];
            printf("%p: %s\n", tempfunc->name, tempfunc->name);
        }
        init();
    }
#pragma endregion DEBUG
    ps->src = line;
    ps->index = 0;
    ps->cc = line[0];
    ps->tok = nextToken(ps->src, &ps->cc, &ps->index);
    base = parseExpr(ps);
    interpret(base, 0, 0);
}

void init() {
    char buffer[256];
    for (;;) {
        printf(">>> ");
        fgets(buffer, 256, stdin);
        exec(buffer);
    }
}

int main(int argc, char** argv) {
    stack = newvoidvector(sizeof(stack_data));
    function_stack = newvoidvector(sizeof(function_stack));
    call_stack = 0; // haven't implemented args fully yet, temp value will be NULL
    init();
}