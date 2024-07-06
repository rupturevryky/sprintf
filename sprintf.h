#ifndef SPRINTFLIBS
#define SPRINTFLIBS

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int minus, offset, need_pluse, space, zero, star, point;

  int h, l, L;
} Flags;

#endif

#ifndef ERRORS
#define ERRORS

#include "errors.c"

#endif

#ifndef WRITE
#define WRITE

void add_char(char **s, char ch, int *count);
void delate_point(char *arr);
void reverse_arr(char *arr);

#include "write.c"

#endif

#ifndef MYMATH
#define MYMATH

#include <math.h>
void mathematical_flags(char **s, int *count, Flags *flags);
#include "myMath.c"

#endif

#ifndef MYFLAGS
#define MYFLAGS

int str_to_int(char *num);
#include <limits.h>

#include "flags.c"

#endif