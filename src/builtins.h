#ifndef _BUILTINS_H
#define _BUILDINS_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

void _writefd(char* fd, char* block);

void _readfd(char* fd);
#endif