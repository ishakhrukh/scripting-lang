#include "interpreter.h"

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
    case A_FORLOOP:
        interpretForLoop(ast, _v);
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

void getVal(AST* variable, voidvector* _stack) {
    stack_data* var, * list_element;
    char* vname;
    AST* list_elem;
    int i, j;
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
                if (var->type == A_LIST) {
                    fprintf(stderr, "\033[1;31mError:\033[0m Unable to get value of variables of type `list`. Please read about lists in the Tutorial in 'readme.md'.\n");
                    init();
                }
                else {
                    if (!variable->value)
                        variable->value = malloc(sizeof(AST));
                    variable->value->cval = var->value;
                    variable->value->type = var->type;
                }
                goto L1;
            }
        }
    }
    for (i = 0; i < stack->size; ++i) {
        var = stack->data[i];
        if (strcmp(var->id, vname) == 0) {
            if (var->type == A_LIST) {
                fprintf(stderr, "\033[1;31mError:\033[0m Unable to get value of variables of type `list`. Please read about lists in the Tutorial in 'readme.md'.\n");
                init();
                /*if (!variable->value)
                    variable->value = newAST(A_LIST);
                //variable->value->children = var->elements;
                list_elem = malloc(sizeof(AST));
                for (j = 0; j < var->elements->size; ++j) {
                    list_element = var->elements->data[j];
                    list_elem->cval = malloc(strlen(list_element->value));
                    strcpy(list_elem->cval, list_element->value);
                    list_elem->type = list_element->type;
                    vvadd(variable->value->children, list_elem);
                }*/
            }
            else {
                if (!variable->value)
                    variable->value = malloc(sizeof(AST));
                variable->value->cval = var->value;
                variable->value->type = var->type;
            }
            goto L1;
        }
    }
    _throw_exception("DefinitionNotFoundException", 0, "Variable definition for '%s' not found\n", variable->value->name);
L1:;
}

void interpretAssignment(AST* ast, voidvector* ps) {
    stack_data* var, * list_elem, * tmp;
    AST* list_element;
    int i;
    short s; // <-- temp
    var = malloc(sizeof(stack_data));
    var->type = ast->value->type;
    var->id = malloc((strlen(ast->name) + 1));
    strcpy(var->id, ast->name);
    if (ps) {
        for (i = 0; i < ps->size; ++i) {
            tmp = ps->data[i];
            if (strcmp(tmp->id, ast->name) == 0)
                vvpop(ps, i);
        }
    }
    else {
        for (i = 0; i < stack->size; ++i) {
            tmp = stack->data[i];
            if (strcmp(tmp->id, ast->name) == 0)
                vvpop(stack, i);
        }
    }
    if (ast->value->type == A_CALL) {  // handle call if the variable's value is a function call (eg. `var = somefunction()`)
        interpretCall(ast->value, ps);
        // returned values from functions are stored in _last_return;
        var->value = malloc((strlen(_last_return) + 1));
        strcpy(var->value, _last_return);
        // the data type of the returned value is stored in _last_return_datatype;
        var->type = _last_return_datatype;
    }
    else if (ast->value->type == A_LIST) {  // handle list if the variable's value is a list (eg. `var = [10, 20, 'str', 3.14]`)
        var->value = 0;
        var->elements = newvoidvector(sizeof(stack_data*));
        for (i = 0; i < ast->value->children->size; ++i) {
            list_element = ast->value->children->data[i];
            list_elem = malloc(sizeof(stack_data));
            list_elem->value = 0;
            if (list_element->type == A_CALL) {
            }
            else if (list_element->type == A_VARIABLE) {
                getVal(list_element, ps);
                list_elem->value = realloc(list_elem, strlen(list_element->value->cval) + 1);
                strcpy(list_elem->value, list_element->value->cval);
            }
            else if (list_element->type == A_INTEGER || list_element->type == A_FLOAT ||
            list_element->type == A_STRING) {
                list_elem->value = realloc(list_elem, strlen(list_element->cval) + 1);
                s = list_element->type;
                list_elem->type = s;
                strcpy(list_elem->value, list_element->cval);
            }
            else 
                _throw_exception("BadInitException", 0, "Invalid initializer for list element in list '%s'. Bad initializer found at index %d\n", ast->name, i);
            vvadd(var->elements, list_elem);
        }
    }
    else if (ast->value->type == A_VARIABLE) {  // handle variable if the variable's value is another variable (eg. `var = anothervar`)
        getVal(ast->value, ps);
        var->value = malloc(strlen(ast->value->value->cval) + 1);
        strcpy(var->value, ast->value->value->cval);
    }
    else if (ast->value->type == A_INTEGER || ast->value->type == A_FLOAT || ast->value->type == A_STRING) {  // handle literal if the variable's value is a literal (eg. `var = 10`, `var = 'some string'`)
        var->value = malloc(strlen(ast->value->cval) + 1);
        strcpy(var->value, ast->value->cval);
    }
    /*else if (ast->value->type == A_LIST_ELEMENT) {
        if (ast->value->flag == A_VARIABLE) {
            getVal(ast->value, ps);
            getListVal(ast->value->value, ps);
            var->value = malloc(strlen(ast->value->value->value->cval) + 1);
            strcpy(var->value, ast->value->value->value->cval);
        }
        else if (ast->value->value->type == A_INTEGER) {
            getListVal(ast->value->value, ps);
            var->value = malloc(strlen(ast->value->value->value->cval) + 1);
            strcpy(var->value, ast->value->value->value->cval);
        }
        else {
            fprintf(stderr, "\033[1;31mError:\033[0m invalid type for list index\n");
            init();
        }
    }*/
    else {
        _throw_exception("BadInitException", 0, "Invalid initializer for variable '%s'\n", ast->name);
    }
    if (ps)
        vvadd(ps, var);
    else
        vvadd(stack, var);
}

void interpretCall(AST* call, voidvector* _ps) {
    voidvector* private_stack = newvoidvector(sizeof(stack_data));
    stack_data* arg;
    AST* a0, * a1;
    int i, j;
    if (handleBuiltin(call, _ps))
        goto L1;
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
                            arg->type = a1->value->type;
                        }
                        else {
                            arg->value = a1->cval;
                            arg->type = a1->type;
                        }
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
                        if (a1->value->type == A_INTEGER ||
                        a1->value->type == A_FLOAT   ||
                        a1->value->type == A_STRING) {
                            getVal(a1, private_stack);
                            RETURNS(a1->value->cval, a1->value->type)
                            goto L1;
                        }
                        else
                            RETURNS(a1->value->cval, a1->flag)
                        goto L1;
                    }
                    interpret(a1, private_stack);
                }
                goto L1;
            }
        }
    _throw_exception("ErrorDefinitionNotFoundException", 0, "Function '%s' is not defined.\n", call->name);
L1:;
}


void interpretForLoop(AST* loop, voidvector* ps) {
    AST* iterator, * target, * step, * instruction;
    stack_data* tmpvar;
    int i, j;
    /*
    LOOP DATA:
    iterator, target, step, instructions
    note: if step is 0(NULL), it is uninitialized, defaulting to '1'
    */
    iterator = loop->children->data[0];
    interpretAssignment(iterator, ps);
    target = loop->children->data[1];
    if (target->type == A_VARIABLE) {
        if (ps) {
            for (i = 0; i < ps->size; ++i) {
                tmpvar = ps->data[i];
                if (strcmp(target->name, tmpvar->id) == 0) {
                    target->cval = malloc(strlen(tmpvar->value));
                    strcpy(target->cval, tmpvar->value);
                    goto L1;
                }
            }
        }
        for (i = 0; i < stack->size; ++i) {
            tmpvar = stack->data[i];
            if (strcmp(target->name, tmpvar->id) == 0) {
                target->cval = malloc(strlen(tmpvar->value));
                strcpy(target->cval, tmpvar->value);
                goto L1;
            }
        }
        _throw_exception("DefinitionNotFoundException", 0, "Variable definition for '%s' not found\n", target->name);
    }
L1:
    if (loop->children->data[2] != 0) {
        step = loop->children->data[2];
        if (step->type == A_VARIABLE) {
            if (ps) {
                for (i = 0; i < ps->size; ++i) {
                    tmpvar = ps->data[i];
                    if (strcmp(step->name, tmpvar->id) == 0) {
                        step->cval = malloc(strlen(tmpvar->value));
                        strcpy(step->cval, tmpvar->value);
                        goto L2;
                    }
                }
            }
            for (i = 0; i < stack->size; ++i) {
                tmpvar = stack->data[i];
                if (strcmp(step->name, tmpvar->id) == 0) {
                    step->cval = malloc(strlen(tmpvar->value));
                    strcpy(step->cval, tmpvar->value);
                    goto L2;
                }
            }
            _throw_exception("DefinitionNotFoundException", 0, "Variable definition for '%s' not found\n", step->name);
        }
    }
    else {
        step = malloc(sizeof(step));
        step->cval = malloc(1);
        strcpy(step->cval, "1");
    }
L2:
    if (loop->flag == LOOP_TO) {
        for (;;) {
            iterator->value->cval = _nsum(iterator->value->cval, step->cval);
            chval(iterator->name, iterator->value->cval, ps);
            if (_ncmp(iterator->value->cval, target->cval) == 0)
                break;
            for (j = 3; j < loop->children->size; ++j) {
                instruction = loop->children->data[j];
                if (instruction->type == A_BREAK)
                    break;
                interpret(instruction, ps);
            }
        }
    }
    else if (loop->flag == LOOP_DOWNTO) {
        printf("Error: Unfortunately, you can't iterate backwards yet, because integers are stored as `char*` in the C code."
        " While this means integers can have no limits (besides how much to OS allows), it has its drawbacks."
        " I've written a function to add 2 `char*`s, treating them as integers, but I haven't yet done the same to subtract them."
        " The feature may become available in the future.\n");
        init();
    }
}

char* _nsum(char* str, char* add) {
    int pos1 = strlen(str), pos2 = strlen(add), pos3, len3;
    unsigned int carry;
    len3 = pos3 = (pos1 < pos2) ? pos2 + 1 : pos1 + 1;
    str = realloc(str, len3 + 1);
    str[pos3] = '\0';
    for (carry = 0; pos3 > 0; carry /= 10) {
        if (pos1 > 0)
            carry += str[--pos1] - '0';
        if (pos2 > 0)
            carry += add[--pos2] - '0';
        str[--pos3] = '0' + carry % 10;
    }
    while (str[0] == '0' && len3 > 1)
        memmove(str, str + 1, len3--);
    return str;
}

// _ncmp: compare two strings as an integer
// returns: 0 if s1 is greater, 1 if s2 is greater, 2 if s1 is equal to s2
int _ncmp(char* s1, char* s2) {
    int l1 = strlen(s1), l2 = strlen(s2), i;
    if (l1 > l2)
        return 0;
    else if (l1 < l2)
        return 1;
    else if (l1 == l2) {
        for (i = 0; i < l1; ++i) {
            if ((s1[i] - '0') < (s2[i] - '0'))
                return 1;
        }
        return 0;
    }
    return 2;
}

void chval(char* vname, char* new, voidvector* ps) {
    stack_data* var;
    int i;
    if (ps) {
        for (i = 0; i < ps->size; ++i) {
            var = ps->data[i];
            if (strcmp(vname, var->id) == 0) {
                var->value = realloc(var->value, strlen(new));
                strcpy(var->value, new);
                vvpop(ps, i);
                vvadd(ps, var);
                break;
            }
        }
    }
    else {
        for (i = 0; i < stack->size; ++i) {
            var = stack->data[i];
            if (strcmp(vname, var->id) == 0) {
                var->value = realloc(var->value, strlen(new));
                strcpy(var->value, new);
                vvpop(stack, i);
                vvadd(stack, var);
                break;
            }
        }
    }
}

int a2i(char* s) {
    int n = 1, num = 0;
    if (*s == '-') {
        n = -1;
        s++;
    }
    while (*s) {
        if (!isdigit(*s)) {
            fprintf(stderr, "string to int convertion failed\n"); exit(1);
        }
        num = ((*s) - '0') + num * 10;
        s++;
    }
    return num * n;
}