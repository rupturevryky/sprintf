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
  } else if (**f == 'g' || **f == 'G') {
    write_shortest_representation(s, args, count);
    *f += 1;
  } else if (**f == 'o') {
    if (write_unsigned_octal(s, args, count) != 0) return 1;
    *f += 1;
  } else if (**f == 's') {
    write_string(s, args, count);
    *f += 1;
  } else if (**f == 'u') {
    if (write_unsigned_int(s, args, count) != 0) return 1;
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
        exit(1);
      }
    } else {
      add_char(&s, *f, &count);
      f++;
    }
  }
  *s = '\0';
  va_end(now_arg);
  return count;
}

int main() {
  int num = 0;
  float fl = 1.012341;
  // double e = 99912355.122349998;
  // double e = 0.00012;
  // double e = 0.00000000000000123456789123456789;
  // double e = 9.99999998;
  // double e = 99.999999999999;
  double e = 0.00000123456789;
  // double e = 0.000000000000123456989123456789;
  // double g = 0.00000593458789;
  // double g = 0.00000500000093458789;
  // double g = 0.;
  // double g = 99.99999998;
  // double g = 0.00012;
  double g = 9912345.122349998;
  // long int octal = 1.5;
  long int octal = 2247483648;
  // long int octal = -1;
  unsigned int u = 14;

  char ch = 'A';
  char str[] = "[str]";

  char input[150];
  const char *format = "%%,d-%d; c-%c; f-%f; s-%s; e-%e; g-%g; o-%o; u-%u";

  // printf("data: %d, %c, %f, %s, %f, %f\n", num, ch, fl, str, e, e);
  // printf("format: %s\n", format);

  int co = s21_sprintf(input, format, num, ch, fl, str, e, g, octal, u);
  printf("s21_    input: %s\n", input);
  printf("Кол-во: %d\n", co);

  co = sprintf(input, format, num, ch, fl, str, e, g, octal, u);

  printf("sprintf input: %s\n", input);
  printf("Кол-во: %d\n", co);

  return 0;
}