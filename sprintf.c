#include "sprintf.h"

int write(const char **f, char **s, va_list *args, int *count) {
  if (**f == 'c') {
    if (write_char(s, args, count) != 0) return 1;
    *f += 1;
  } else if (**f == 'd' || **f == 'i') {
    if (write_int(s, args, count) != 0) return 1;
    *f += 1;
  } else if (**f == 'e' || **f == 'E') {
    write_scientific_notation(s, args, count, **f);
    *f += 1;
  } else if (**f == 'f') {
    write_float(s, args, count);
    *f += 1;
  } else if (**f == 's') {
    write_string(s, args, count);
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
  float fl = 1.012341;
  double e = 0.0000000000000123456789123456789;
  // double e = 9.99999998;
  // double e = 0.99999998;
  // double e = 0.00000123456789;

  char ch = 'A';
  char str[] = "[str]";

  char input[100];
  const char *format = "%%,d-%d; c-%c; f-%f; s-%s; e-%e";

  printf("data: %d, %c, %f, %s, %f\n", num, ch, fl, str, e);
  printf("format: %s\n", format);

  int co = s21_sprintf(input, format, num, ch, fl, str, e);
  printf("s21_    input: %s\n", input);
  printf("Кол-во: %d\n", co);

  co = sprintf(input, format, num, ch, fl, str, e);
  printf("sprintf input: %s\n", input);
  printf("Кол-во: %d\n", co);

  return 0;
}