#ifndef SPRINTFLIBS
#define SPRINTFLIBS

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif

#ifndef WRITE
#define WRITE

void add_char(char **s, char ch, int *count);
void delate_point(char *arr);

#include "write.c"

#endif

#ifndef MYMATH
#define MYMATH

#include <math.h>

#include "myMath.c"

#endif