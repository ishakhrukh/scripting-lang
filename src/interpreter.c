#include "interpreter.h"

void interpretCompound(AST* ast, void* _struct, short flags) {
    int i;
    for (i = 0; i < ast->children->size; ++i)
        interpret(ast->children->data[i], _struct, flags);
}

void interpretAssignment(AST* ast, void* _struct, short flags) {
    stack_data* var;
    var = malloc(sizeof(stack_data));
    var->type = 0;
    var->id = malloc((strlen(ast->name) + 1) * sizeof(char));
    var->value = malloc((strlen(ast->value->cval) + 1) * sizeof(char));
    strcpy(var->id, ast->name);
    strcpy(var->value, ast->value->cval);
    if (flags == 1)
        vvadd(_struct, var);
    else
        vvadd(stack, var);
}

void interpretCall(AST* function) {
    voidvector* private_stack = newvoidvector(sizeof(stack_data));
    AST* temp;
    int i, j;
    // load args somehow
    // in progress
    for (i = 0; i < function->argcount; ++i) {
        vvadd(private_stack, 0); // TODO: 
        //                          find out how to read args
    }
    for (i = 0; i < function_stack->size; ++i) {
        temp = function_stack->data[i];
        if (strcmp(temp->name, function->name) == 0) {
            for (j = 0; j < temp->children->size; ++j) {
                interpret(temp->children->data[i], private_stack, 1);
            }
            goto done;
        }
    }
    printf("Error: Definition for function `%s' not found\n", function->name);
    init();
    done:;
}

void interpret(AST* ast, void* _struct, short flags) {
    switch (ast->type) {
    case A_CALL:
        interpretCall(ast);
        break;
    case A_COMPOUND:
        interpretCompound(ast, _struct, flags);
        break;
    case A_ASSIGNMENT:
        interpretAssignment(ast, _struct, flags);
        break;
    case A_FUNCTION:
        vvadd(function_stack, ast);
        break;
    case A_VARIABLE:
        break;
    default:
        printf("Error: Traceback: ast->type = %d\n", ast->type);
        init();
    }
}