#include "interpreter.h"

void getVal(AST* variable, voidvector* _stack) {
    stack_data* var;
    char* vname;
    int i;
    if (variable->name) {
        vname = malloc(strlen(variable->name));
        strcpy(vname, variable->name);
    }
    else if (variable->value) {
        if (variable->value->name) {
            vname = malloc(strlen(variable->value->name));
            strcpy(vname, variable->value->name);
        }
        else {
            printf("the 'impossible' happened: variable to check has no name/id\n");
            exit(1);
        }
    }
    else {
        printf("the 'impossible' happened: variable to check has no name/id\n");
            exit(1);
    }
    if (_stack) {
        for (i = 0; i < _stack->size; ++i) {
            var = _stack->data[i];
            if (strcmp(var->id, vname) == 0) {
                if (!variable->value)
                    variable->value = malloc(sizeof(AST));
                variable->value->cval = var->value;
                variable->value->type = var->type;
                goto done_at_getval;
            }
        }
    }
    for (i = 0; i < stack->size; ++i) {
        var = stack->data[i];
        if (strcmp(var->id, vname) == 0) {
            if (variable->value == 0)
                variable->value = malloc(sizeof(AST));
            variable->value->cval = var->value;
            variable->value->type = var->type;
            goto done_at_getval;
        }
    }
    _throw_exception("DefinitionNotFoundException", 0, "Variable definition for '%s' not found\n", variable->value->name);
done_at_getval:;
}

void interpretAssignment(AST* ast, void* _v) {
    stack_data* var;
    int i;
    var = malloc(sizeof(stack_data));
    var->type = ast->value->type;
    var->id = malloc((strlen(ast->name) + 1));
    strcpy(var->id, ast->name);
    if (ast->value->type == A_CALL) {  // handle call if the variable's value is a function call
        interpretCall(ast->value, _v);
        // returned values from functions are stored in _last_return;
        var->value = malloc((strlen(_last_return) + 1));
        strcpy(var->value, _last_return);
    }
    else {  // else, the value will be a constant or a variable
        if (ast->value->type == A_VARIABLE) {
            getVal(ast->value, _v);
            var->value = malloc((strlen(ast->value->value->cval) + 1));
            strcpy(var->value, ast->value->value->cval);
        }
        else {
            var->value = malloc((strlen(ast->value->cval) + 1));
            strcpy(var->value, ast->value->cval);
        }
    }
    if (_v)
        vvadd(_v, var);
    else
        vvadd(stack, var);
}

void interpretCall(AST* call, voidvector* _ps) {
    voidvector* private_stack = newvoidvector(sizeof(stack_data));
    stack_data* arg;
    AST* a0, * a1;
    int i, j;
    if (handleBuiltin(call, _ps))
        goto done_at_ic;
    for (i = 0; i < function_stack->size; ++i) {
            a0 = function_stack->data[i];
            if (strcmp(a0->name, call->name) == 0) {
                if (call->argcount == a0->argcount) {
                    // load arguments onto the function's private stack
                    for (j = 0; j < call->argcount; ++j) {
                        arg = malloc(sizeof(stack_data));
                        a1 = a0->children->data[j];
                        arg->id = a1->name;
                        a1 = call->children->data[j];
                        if (a1->type == A_VARIABLE) {
                            getVal(a1, _ps);
                            arg->value = a1->value->cval;
                        }
                        else
                            arg->value = a1->cval;
                        vvadd(private_stack, arg);
                    }
                }
                else {
                    _throw_exception("IllegalArgumentException", 0, "Too %s arguments passed to function '%s', which takes %d parameters\n", 
                    call->argcount > a0->argcount ? "many" : "few", a0->name, a0->argcount);
                    init();
                }
                for (j = call->argcount; j < a0->children->size; ++j) {
                    a1 = a0->children->data[j];
                    if (a1->value) {
                        // sometimes, the syntax tree won't have a `value`, thus resulting
                        // in a segfault when trying to access its type, therefore we have
                        // to look out for that
                        if (a1->value->type == A_VARIABLE)
                            getVal(a1, private_stack);
                    }
                    if (a1->type == A_RETURN) {
                        if (a1->value->type != A_INTEGER ||
                            a1->value->type != A_FLOAT   ||
                            a1->value->type != A_INTEGER)
                            getVal(a1, private_stack);
                        returns(a1->value->cval);
                        goto done_at_ic;
                    }
                    interpret(a1, private_stack);
                }
                goto done_at_ic;
            }
        }
    _throw_exception("ErrorDefinitionNotFoundException", 0, "Function '%s' is not defined.\n", call->name);
done_at_ic:;
}

void interpret(AST* ast, void* _v) {
    if (ast == 0)
        goto end;
    switch (ast->type) {
    case A_CALL:
        interpretCall(ast, _v);
        break;
    case A_ASSIGNMENT:
        interpretAssignment(ast, _v);
        break;
    case A_FUNCTION:
        vvadd(function_stack, ast);
        break;
    default:
        printf("Uh-oh: No support for ast of type %d\n", ast->type);
        init();
    }
end:;
}