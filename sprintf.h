#ifndef SPRINTFLIBS
#define SPRINTFLIBS

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#endif

#ifndef WRITE
#define WRITE

#include "write.c"

void add_char(char **s, char ch, int *count);
int write_int(char **s, va_list *args, int *count);
int write_char(char **s, va_list *args, int *count);
int write_float(char **s, va_list *args, int *count);

#endif

#ifndef MYMATH
#define MYMATH

#include "myMath.c"

int increase_discharge(int exp);
int take_len_of_num(int num, int now);
int getFirstDigit(int number);

#endif