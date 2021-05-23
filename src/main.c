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
    if (strcmp(line, "$?\n") == 0 || strcmp(line, "$?") == 0) {
        printf("%s\n", _last_return);
        init();
    }
#pragma endregion DEBUG
    ps->src = line;
    ps->index = 0;
    ps->cc = line[0];
    ps->tok = nextToken(ps->src, &ps->cc, &ps->index);
    base = parse(ps);
    interpret(base, 0);
}

// user-defined getline function for portability with Windows when I use msvc
size_t _getline(char** lineptr, size_t* __n, FILE* stream) {
    char *bufptr = 0;
    char *p = bufptr;
    size_t size;
    int c;
    if (lineptr == 0 || stream == 0 || __n == 0)
        return -1;
    bufptr = *lineptr;
    size = *__n;
    c = fgetc(stream);
    if (c == -1)
        return -1;
    if (bufptr == 0) {
        bufptr = malloc(128);
        if (bufptr == 0)
            return -1;
        size = 128;
    }
    p = bufptr;
    while (c != -1) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == 0)
                return -1;
        }
        *p++ = c;
        if (c == '\n')
            break;
        c = fgetc(stream);
    }
    *p++ = 0;
    *lineptr = bufptr;
    *__n = size;
    return p - bufptr - 1;
}

void init() {
    char* buffer = 0;
    size_t len = 0;
    if (_inputbuf != stdin) {  // if the input buffer is not stdin, we need to use a different method to get lines
        while (_getline(&buffer, &len, _inputbuf) != -1)
            exec(buffer);
        exit(0);
    }
    else {
        for (;;) {
            printf(">>> ");
            _getline(&buffer, &len, stdin);
            exec(buffer);
        }
    }
}

int main(int argc, char** argv) {
    stack = newvoidvector(sizeof(stack_data));
    function_stack = newvoidvector(sizeof(AST));
    _last_return = calloc(1, sizeof(char));
    if (argc < 2)
        _inputbuf = stdin;
    else {
        /*_inputbuf = fopen(argv[1], "rb");
        if (_inputbuf == 0) {
            fprintf(stderr, "\033[1;31mError:\033[0m Failed to open script file '%s'\nPress ENTER to continue to the shell\n", argv[1]);
            getchar();
            _inputbuf = stdin;
        }*/
        _inputbuf = stdin;
    }
    init();
}