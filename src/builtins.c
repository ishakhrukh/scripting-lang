#include "builtins.h"

sl_param* sp_create(char* value, short type) {
    sl_param* _ = malloc(sizeof(sl_param));
    _->value = malloc(strlen(value));
    strcpy(_->value, value);
    _->type = type;
    return _;
}

int handleBuiltin(AST* call, voidvector* fstack) {
    sl_param** args;
    AST* arg;
    int i;
    if (strcmp(call->name, "_write") == 0) {
        if (call->argcount != 2)
            _throw_exception("IllegalArgumentException", 0, "Too %s arguments passed to function `_writef(dest, block)`, which takes 2 argument(s) (%d arguments were passed).\n", call->argcount < 2 ? "few" : "many", call->argcount);
        args = malloc(2 * sizeof(sl_param*));
        for (i = 0; i < 2; ++i) {
            arg = call->children->data[i];
            if (arg->type == A_VARIABLE) {
                getVal(arg, fstack);
                args[i] = sp_create(arg->value->cval, arg->value->type);
            }
            else
                args[i] = sp_create(arg->cval, arg->type);
        }
        _write(args[0], args[1]);
        free(args[0]);
        free(args[1]);
        free(args);
        return 1;
    }
    else if (strcmp(call->name, "_read") == 0) {
        if (call->argcount != 1)
            _throw_exception("IllegalArgumentException", 0, "Too %s arguments passed to function `_read(src)`, which takes 1 argument(s) (%d arguments were passed).\n", call->argcount < 2 ? "few" : "many", call->argcount);
        args = malloc(sizeof(sl_param*));
        arg = call->children->data[0];
        if (arg->type == A_VARIABLE) {
            getVal(arg, fstack);
            args[0] = sp_create(arg->value->cval,arg->value->type);
        }
        else
            args[0] = sp_create(arg->cval, arg->type);
        _read(args[0]);
        free(args[0]);
        free(args);
        return 1;
    }
    else if (strcmp(call->name, "_nextLine") == 0) {
        if (call->argcount != 0)
            _throw_exception("IllegalArgumentException", 0, "Too many arguments passed to function `_nextLine()`, which takes 0 argument(s) (%d arguments were passed).\n", call->argcount);
        _nextLine();
        return 1;
    }
    else if (strcmp(call->name, "_memdump") == 0) {
        if (call->argcount != 0)
            _throw_exception("IllegalArgumentException", 0, "Too many arguments passed to function `_memdump()`, which takes 0 argument(s) (%d arguments were passed).\n", call->argcount);
        _memdump();
        return 1;
    }
    else if (strcmp(call->name, "_exit") == 0) {
        if (call->argcount != 1)
            _throw_exception("IllegalArgumentException", 0, "Too many arguments passed to function `_exit(status)`, which takes 0 argument(s) (%d arguments were passed).\n", call->argcount);
        args = malloc(sizeof(sl_param*));
        arg = call->children->data[0];
        if (arg->type == A_VARIABLE) {
            getVal(arg, fstack);
            args[0] = sp_create(arg->value->cval,arg->value->type);
        }
        else
            args[0] = sp_create(arg->cval, arg->type);
        builtin_exit(args[0]);
        free(args[0]);
        free(args);
        return 1;
    }
    return 0;
}

void _write(sl_param* dest, sl_param* block) {
    FILE* stream;
    /*if (block->type != A_STRING) {
        _throw_exception("IllegalArgumentException", 0, "Invalid argument(s) passed to function `_write(dest, block)`, where `dest` is `int` or `string`, and `block` is `string`\n");
    }*/
    if (dest->type == A_INTEGER) {
        switch (dest->type) {
        case 1:
            fwrite(block->value, sizeof(char), strlen(block->value), stdout);
            break;
        case 2:
            fwrite(block->value, sizeof(char), strlen(block->value), stderr);
            break;
        default:
            fwrite(block->value, sizeof(char), strlen(block->value), stdout);
        }
    }
    else if (dest->type == A_STRING) {
        stream = fopen(dest->value, "wb");
        if (stream == 0)
            _throw_exception("IOException", 1, "An error occoured while trying to open file '%s'\n'%s': ", dest->value, dest->value);
        fwrite(block->value, sizeof(char), strlen(block
    ->value), stream);
        fclose(stream);
    }
}

void _read(sl_param* src) {
    FILE* stream;
    size_t size;
    if (src->type != A_STRING)
        _throw_exception("IllegalArgumentException", 0, "Invalid argument passed to function `_read(src)` where `src` is `string`\n");
    stream = fopen(src->value, "rb");
    if (stream == 0)
        _throw_exception("IOException", 1,  "An exception occoured while trying to open file '%s'\n%s: ", src->value, src->value);
    fseek(stream, 0, SEEK_END);
    size = ftell(stream);
    free(_last_return);
    _last_return = malloc(size);
    rewind(stream);
    fread(_last_return, size, sizeof(char), stream);
    fclose(stream);
}

void _nextLine() {
    char* buffer, c;
    size_t len = 0;
    for (c = fgetc(stdin); c != '\n'; c = fgetc(stdin)) {
        len += 1;
        buffer = realloc(buffer, len);
        buffer[len - 1] = c;
    }
    buffer[len] = 0;
    returns(buffer);
}

void _memdump() {
    stack_data* var;
    char* value;
    int i;
    for (i = 0; i < stack->size; ++i) {
        var = stack->data[i];
        printf("[%s : ", var->id);
        if (var->type == A_STRING)
            printf("'%s']\n", var->value);
        else
            printf("%s]\n", var->value);
    }
}

void builtin_exit(sl_param* status) {
    if (status->type != A_INTEGER)
        _throw_exception("IllegalArgumentException", 0, "Invalid argument passed to function `_exit(status)` where `status` is `int`\n");
    exit(atoi(status->value));
}

void _throw_exception(char* exception, short pe, char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    fprintf(stderr, "\033[1;31mAn exception occoured:\033[0m\n%s\nDetails: ", exception);
    vfprintf(stderr, msg, ap);
    if (pe)
        perror(0);
    va_end(ap);
    init();
}
