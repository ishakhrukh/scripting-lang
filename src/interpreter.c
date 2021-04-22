#include "interpreter.h"

void getVal(AST* variable, voidvector* _stack) {
    AST* ret;
    stack_data* var;
    int i;
    for (i = 0; i < _stack->size; ++i) {
        var = _stack->data[i];
        if (strcmp(var->id, variable->value->name) == 0) {
            variable->value->cval = var->value;
            goto done;
        }
    }
    for (i = 0; i < stack->size; ++i) {
        var = stack->data[i];
        if (strcmp(var->id, variable->value->name) == 0) {
            variable->value->cval = var->value;
            goto done;
        }
    }
    printf("Error: variable definition for `%s' not found\n", variable->value->name);
done
:;
}

void interpretAssignment(AST* ast, void* _struct, short flags) {
    stack_data* var;
    var = malloc(sizeof(stack_data));
    var->type = 0;
    var->id = malloc((strlen(ast->name) + 1) * sizeof(char));
    strcpy(var->id, ast->name);
    if (ast->value->type == A_CALL) {
        interpretCall(ast->value);
        var->value = malloc((strlen(_f_retval) + 1) * sizeof(char));
        strcpy(var->value, _f_retval);
    }
    else {
        var->value = malloc((strlen(ast->value->cval) + 1) * sizeof(char));
        strcpy(var->value, ast->value->cval);
    }
    if (flags == 1) {
        vvadd(_struct, var);
    }
    else {
        vvadd(stack, var);
    }
}

void interpretCall(AST* call) {
    voidvector* private_stack = newvoidvector(sizeof(stack_data));
    stack_data* arg;
    AST* a0, * a1;
    int i, j;
    for (i = 0; i < function_stack->size; ++i) {
            a0 = function_stack->data[i];
            if (strcmp(a0->name, call->name) == 0) {
                if (call->argcount == a0->argcount) {
                    for (j = 0; j < call->argcount; ++j) {
                        arg = malloc(sizeof(stack_data));
                        a1 = a0->children->data[j];
                        arg->id = a1->name;
                        a1 = call->children->data[j];
                        arg->value = a1->cval;
                        vvadd(private_stack, arg);
                    }
                }
                else {
                    printf("Error: too %s arguments passed to function '%s', which takes %d parameters\n", 
                    call->argcount > a0->argcount ? "many" : "few", a0->name, a0->argcount);
                    init();
                }
                for (j = call->argcount; j < a0->children->size; ++j) {
                    a1 = a0->children->data[j];
                    if (a1->value->type == A_VARIABLE) {
                        getVal(a1, private_stack);
                    }
                    if (a1->type == A_RETURN) {
                        if (a1->value->type == A_VARIABLE) {
                            getVal(a1, private_stack);
                            _f_retval = realloc(_f_retval, strlen(a1->value->cval) * sizeof(char));
                            memset(_f_retval, 0, strlen(a1->value->cval));
                            strcpy(_f_retval, a1->value->cval);
                            goto done;
                        }
                        _f_retval = realloc(_f_retval, strlen(a1->value->cval) * sizeof(char));
                        memset(_f_retval, 0, strlen(a1->value->cval));
                        strcpy(_f_retval, a1->value->cval);
                        goto done;
                    }
                    interpret(a1, private_stack, 1);
                }
                goto done;
            }
        }
    printf("Error: Definition for function `%s' not found\n", call->name);
    init();
done
:;
}

void interpret(AST* ast, void* _struct, short flags) {
    if (ast == 0) {
        goto end;
    }
    switch (ast->type) {
    case A_CALL:
        interpretCall(ast);
        break;
    case A_ASSIGNMENT:
        interpretAssignment(ast, _struct, flags);
        break;
    case A_FUNCTION:
        vvadd(function_stack, ast);
        break;
    default:
        printf("Error: Traceback: ast->type = %d\n", ast->type);
        init();
    }
end
:;
}