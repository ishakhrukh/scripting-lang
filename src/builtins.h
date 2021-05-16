#ifndef _BUILTINS_H
#define _BUILTINS_H
#include <stdarg.h>
#include "interpreter.h"

typedef stack_data sl_param;

sl_param* sp_create(char* value, short type);

int handleBuiltin(AST* call, voidvector* fstack);

void _write(sl_param* dest, sl_param* block);

void _read(sl_param* src);

void _nextLine();

void _memdump();

void builtin_exit(sl_param* status);

void _throw_exception(char* exception, short pe, char* msg, ...);
#endif