#include "sprintf.h"

int check_errors(char ch, Flags *flags) {
  int res = 0;
  if (ch == 'c' || ch == 's' || ch == 'p' || ch == 'n' || ch == 'o' ||
      ch == 'u' || ch == 'x' || ch == 'X') {
    if (flags->space) {
      fprintf(stderr,
              "\033[31;1merror:\033[0m ' ' flag used with ‘%%%c’ gnu_printf "
              "format [\033[31;1m-Werror=format=\033[0m]\n",
              ch);
      res = 1;
    }
    if (flags->need_pluse) {
      fprintf(stderr,
              "\033[31;1merror:\033[0m '+' flag used with ‘%%%c’ gnu_printf "
              "format [\033[31;1m-Werror=format=\033[0m]",
              ch);
      res = 1;
    }
  }
  if (ch == '%' && flags->space) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m conversion lacks type at end of format "
            "[\033[31;1m-Werror=format=\033[0m]\n");
    res = 1;
  }
  return res;
}

int length_description(const char **f, Flags *flags) {
  char ch = **f;
  if (ch == 'h' || ch == 'l' || ch == 'L')
    *f += 1;
  else
    return 0;

  if (ch == 'h') {
    if (**f != 'i' && **f != 'd' && **f != 'o' && **f != 'u' && **f != 'x' &&
        **f != 'X') {
      fprintf(stderr,
              "\033[31;1merror:\033[0m use of ‘h’ length modifier with ‘%c’ "
              "type character has either no effect or undefined behavior "
              "[\033[31;1m-Werror=format=\033[0m]\n",
              **f);
      return 1;
    }
    flags->h = 1;
  } else if (ch == 'l') {
    if (**f != 'i' && **f != 'd' && **f != 'o' && **f != 'u' && **f != 'x' &&
        **f != 'X' && **f != 'c' && **f != 's' && **f != 'l') {
      fprintf(stderr,
              "\033[31;1merror:\033[0m use of ‘l’ length modifier with ‘%c’ "
              "type character has either no effect or undefined behavior "
              "[\033[31;1m-Werror=format=\033[0m]\n",
              **f);
      return 1;
    }
    flags->l += 1;
  } else if (ch == 'L') {
  }
  return length_description(f, flags);
}

int write(const char **f, char **s, va_list *args, int *count) {
  Flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  if (take_flags(f, &flags, 0)) return 1;
  if (check_errors(**f, &flags)) return 1;
  if (length_description(f, &flags)) return 1;

  if (**f == 'c') {
    if (write_char(s, args, count, &flags)) return 1;
  } else if (**f == 'd' || **f == 'i') {
    if (write_int(s, args, count, &flags, **f)) return 1;
  } else if (**f == 'e' || **f == 'E') {
    write_scientific_notation(s, args, count, **f, &flags);
  } else if (**f == 'f') {
    write_float(s, args, count, &flags);
  } else if (**f == 'g' || **f == 'G') {
    write_shortest_representation(s, args, count, &flags);
  } else if (**f == 'o') {
    if (write_unsigned_octal(s, args, count, &flags)) return 1;
  } else if (**f == 's') {
    write_string(s, args, count, &flags);
  } else if (**f == 'u') {
    if (write_unsigned_int(s, args, count, &flags)) return 1;
  } else if (**f == 'x' || **f == 'X') {
    if (write_unsigned_hexadecimal_integer(s, args, count, **f, &flags))
      return 1;
  } else if (**f == 'p') {
    write_ptr_adress(s, args, count, &flags);
  } else if (**f == 'n') {
    write_count(args, count);
  } else if (**f == '%') {
    write_percentage(s, **f, count, &flags);
  }
  *f += 1;
  return 0;
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list now_arg;
  va_start(now_arg, format);
  char *s = str;
  const char *f = format;
  int count = 0, res = 0;

  while (*f != '\0') {
    if (*f == '%') {
      f += 1;
      if (write(&f, &s, &now_arg, &count) != 0) res = 1;
    } else {
      add_char(&s, *f, &count);
      f++;
    }
  }
  *s = '\0';
  va_end(now_arg);
  if (res) exit(1);
  return count;
}

int main() {
  int num = 1;
  long int lnum = 9223372036854775807;
  float fl = 1.012341;
  // double e = 99912355.122349998;
  // double e = 0.00012;
  // double e = 0.00000000000000123456789123456789;
  // double e = 9.99999998;
  // double e = 99.999999999999;
  // double e = 0.00000123456789;
  double e = 0.000000000000123456989123456789;
  double g = 0.00000593458789;
  // double g = 9.99999998;
  // double g = 0.00000500000093458789;
  // double g = 0.;
  // double g = 0.00012;
  // double g = 9912345.122349998;
  // long int octal = 1.5;
  long int octal = 2247483648;
  // long int octal = -1;
  unsigned int u = 32767;
  unsigned int x = 12345678;
  int s21_count = 0;
  int count = 1;

  char ch = 'A';
  char str[] = "[str]";

  char input[200];
  const char *format =
      "%%,d-%d; c-%c; f-%f; s-%s; e-%+e; g-%g; o-%o; u-%u; x-%x; X-%X; n-%n;"
      "p-%p; -5d-|%-3d|; -+10d-|%-+3d|; +10f-|%+10f|; (%% f)-|% f|;";

  const char *length_format =
      "%%ld-%ld; %%lli-%lli; %%lo-%lo; %%llu-%llu; %%lx-%lx; %%llX-%llx";

  s21_sprintf(input, "1: %d; 2: |%-3d|; 3: |%-+3d|; 4: |% d|", num, num, num,
              num);

  s21_sprintf(input, "1: %c; 2: |%c|; 3: |%s|; 4: |%s|", 'A', 'B', "qwe",
              "QWE");
  printf("s21_input: %s\n", input);
  sprintf(input, "1: %c; 2: |%c|; 3: |%s|; 4: |%s|", 'A', 'B', "qwe", "QWE");
  printf("    input: %s\n", input);
  s21_sprintf(input, "1: %f; 2: |%+10f|;", fl, fl);
  printf("s21_input: %s\n", input);
  sprintf(input, "1: %f; 2: |%+10f|;", fl, fl);
  printf("    input: %s\n", input);

  int co = s21_sprintf(input, format, num, ch, fl, str, e, g, octal, u, x, x,
                       &s21_count, &str, num, num, fl, fl);
  printf("s21_    input: %s\n", input);
  printf("s21_count: %d\n", s21_count);
  printf("Кол-во: %d\n", co);
  co = sprintf(input, format, num, ch, fl, str, e, g, octal, u, x, x,
               &s21_count, &str, num, num, fl, fl);
  printf("        input: %s\n", input);
  printf("s21_count: %d\n", s21_count);
  printf("Кол-во: %d\n", co);

  co = s21_sprintf(input, length_format, lnum, u, s21_count, count, u, lnum);
  printf("s21_    input: %s\n", input);
  printf("Кол-во: %d\n", co);

  co = sprintf(input, length_format, lnum, u, s21_count, count, u, lnum);
  printf("sprintf input: %s\n", input);
  printf("Кол-во: %d\n", co);

  return 0;
}