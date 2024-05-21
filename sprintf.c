#include "sprintf.h"

int write(const char **f, char **s, va_list *args, int *count) {
  if (**f == 'd' || **f == 'i') {
    if (write_int(s, args, count) != 0) return 1;
    *f += 1;
  } else if (**f == 'f') {
    if (write_float(s, args, count) != 0) return 1;
    *f += 1;
  } else if (**f == 'c') {
    if (write_char(s, args, count) != 0) return 1;
    *f += 1;
  } else if (**f == '%') {
    add_char(s, **f, count);
    *f += 1;
  }
  return 0;
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list now_arg;
  va_start(now_arg, format);
  char *s = str;
  const char *f = format;
  int count = 0;

  while (*f != '\0') {
    if (*f == '%') {
      f += 1;
      if (write(&f, &s, &now_arg, &count) != 0) {
        va_end(now_arg);
        return 0;
      }
    } else {
      add_char(&s, *f, &count);
      f++;
    }
    // printf("str: %s\n", str);
  }
  *s = '\0';
  va_end(now_arg);
  return count;
}

int main() {
  int num = -1;
  float fl = 0.0123;
  char ch = 'A';
  char ch2 = 'B';

  char input[100];
  const char *format = "%%!%d!-`%c%c`-float:'%f'";

  printf("data: %d, %c, %c, %f\n", num, ch, ch2, fl);

  int co = s21_sprintf(input, format, num, ch, ch2, fl);

  printf("format: %s\n", format);
  printf("input: %s\n", input);

  printf("Кол-во: %d\n", co);

  return 0;
}