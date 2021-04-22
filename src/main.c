#include "tree.h"
#include "parse.h"
#include "stack.h"
#include "token.h"
#include "interpreter.h"
#include <signal.h>

void exec(char* line) {
    parser* ps = malloc(sizeof(parser));
    AST* base;
#pragma region DEBUG    // collapse this block of code so it
                        // doesn't get in my way
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
    if (strcmp(line, "$?\n") == 0) {
        printf("%s\n", _f_retval);
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

void trap() {
    printf("\033[1;31mThe shell experienced a crash and had to terminate. If the cause of the crash was unintentional, please report the bug in `issues`.\033[0m\n");
    exit(1);
}

int main(int argc, char** argv) {
    signal(SIGSEGV, trap);
    stack = newvoidvector(sizeof(stack_data));
    function_stack = newvoidvector(sizeof(AST));
    _f_retval = malloc(sizeof(char));
    memset(_f_retval, 0, 1);
    init();
}
