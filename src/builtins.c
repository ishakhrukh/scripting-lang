#include "builtins.h"

void _writefd(char* fd, char* block) {
    FILE* stream;
    if (strcmp(fd, "stdout") == 0) {
        stream = stdout;
    }
    else {
        stream = fopen(fd, "wb");
    }
    fwrite(block, sizeof(char), strlen(block), stream);
    fclose(stream);
}

void _readfd(char* fd) {
    FILE* stream;
    size_t size;
    if (strcmp(fd, "stdin") == 0) {
        free(_f_retval);
        fgets(_f_retval, 256, stdin);
    }
    else {
        stream = fopen(fd, "rb");
        fseek(stream, 0, SEEK_END);
        size = ftell(stream);
        free(_f_retval);
        _f_retval = malloc(size * sizeof(char));
        rewind(stream);
        fread(_f_retval, size, sizeof(char), stream);
        fclose(stream);
        _f_retval[size - 1] = 0;
    }
}