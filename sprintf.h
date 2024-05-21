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

int write_int(char **s, va_list *args, int *count);
int write_char(char **s, va_list *args, int *count);

#endif

#ifndef MYMATH
#define MYMATH

#include "myMath.c"

int increase_discharge(int exp);
int take_len_of_num(int num, int now);
int getFirstDigit(int number);

#endif

// #ifndef SPRINTF
// #define SPRINTF

// #include "sprintf.c"

// int write(const char **f, char **s, va_list *args, int *count);
// int s21_sprintf(char *str, const char *format, ...);

// #endif