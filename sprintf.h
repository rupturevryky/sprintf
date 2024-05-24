#ifndef SPRINTFLIBS
#define SPRINTFLIBS

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //

#endif

#ifndef WRITE
#define WRITE

#include "write.c"

void add_char(char **s, char ch, int *count);
int write_int(char **s, va_list *args, int *count);
int write_char(char **s, va_list *args, int *count);
void write_float(char **s, va_list *args, int *count);
int write_scientific_notation(char **s, va_list *args, int *count, char E);
int write_string(char **s, va_list *args, int *count);

#endif

#ifndef MYMATH
#define MYMATH

#include <math.h>

#include "myMath.c"
int increase_discharge(int exp);
int getFirstDigit(int number);
int digit_down(int *num, int *tpm_len, int *len, int E, int *count, char **s,
               int should_point);

#endif