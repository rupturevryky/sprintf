#include "sprintf.h"

int write(const char **f, char **s, va_list *args, int *count) {
  Flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  if (take_flags(f, &flags, 0, args)) return 1;
  if (check_errors(**f, &flags)) return 1;
  if (length_description(f, &flags)) return 1;
  // printf(
  //     "minus:%d, need_pluse:%d, offset:%d, space:%d, zero:%d, star:%d, l:%d,
  //     L:%d, h:%d\n", flags.minus, flags.need_pluse,
  //     flags.offset, flags.space, flags.zero, flags.star, flags.l, flags.L,
  //     flags.h);

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

int s21_sprintf(char *str, const char *test_3_format, ...) {
  va_list now_arg;
  va_start(now_arg, test_3_format);
  char *s = str;
  const char *f = test_3_format;
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
  va_end(now_arg);
  if (res) exit(1);
  return count;
}

int main() {
  int num = 12;
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
  int my_s21_count = 0;
  int count = 1;

  char ch = 'A';
  char B = 'B';
  char str[] = "[str]";

  char s21_input[200];
  char input[200];
  int test_number = 0;
  int success = 1;

  /*
  error tests
  */
  /*
  s21_sprintf(s21_input, "1:%d; 2:|%-3d|; 3:|%-+3d|; 4:|% d|", 2247483648, num,
              num, num);
   */
  const char *test_0_format =
      "1:%d; 2:|%-3d|; 3:|%-+3d|; 4:|% d|; 5:|% 15d|; 6:|%+15d|; 7:|%015f|";
  const char *test_1_format = "1: %c; 2: |%c|; 3: |%s|; 4: |%s|";
  const char *test_2_format = "1:%f; 2:|%+10f|; 3:|%- 25f|; 4:|%- 25ld|";
  const char *test_3_format =
      "%%,d-%d; c-%c; f-%f; s-%s; e-%+e; g-%g; o-%o; u-%u; x-%x; X-%X; "
      "n-%n;"
      "p-%p; -5d-|%-3d|; -+10d-|%-+3d|; +10f-|%+10f|; (%% f)-|% f|;";
  const char *test_4_format =
      "%%ld-%ld; %%lli-%lli; %%lo-%lo; %%llu-%llu; %%lx-%lx; %%llX-%llx";
  const char *test_5_format =
      "1:%1d; 2:|%2d|; 3:|%4d|; 4:|% 3d|; 5:|%*d|; 6:|%15d|; 7:|%02f|";

  // test 0
  int s21_co =
      s21_sprintf(s21_input, test_0_format, num, num, num, num, num, num, fl);
  // printf("s21_input: %s; co: %d\n", s21_input, s21_co);
  int co = sprintf(input, test_0_format, num, num, num, num, num, num, fl);
  // printf("    input: %s; co: %d\n", input, co);

  if (strcmp(s21_input, input) != 0 || co != s21_co) {
    printf("test_number: %d\n", test_number);
    success = 0;
  }
  test_number++;
  // test 1
  s21_co = s21_sprintf(s21_input, test_1_format, ch, B, str, "QWE");
  // printf("s21_input: %s; co: %d\n", s21_input, s21_co);
  co = sprintf(input, test_1_format, ch, B, str, "QWE");
  // printf("    input: %s; co: %d\n", input, co);

  if (strcmp(s21_input, input) != 0 || co != s21_co) {
    success = 0;
    printf("test_number: %d\n", test_number);
  }
  test_number++;
  // test 2
  s21_co = s21_sprintf(s21_input, test_2_format, fl, fl, fl, octal);
  // printf("s21_input: %s; co: %d\n", s21_input, s21_co);
  co = sprintf(input, test_2_format, fl, fl, fl, octal);
  // printf("    input: %s; co: %d\n", input, co);

  if (strcmp(s21_input, input) != 0 || co != s21_co) {
    success = 0;
    printf("test_number: %d\n", test_number);
  }
  test_number++;
  // test 3
  s21_co = s21_sprintf(s21_input, test_3_format, num, ch, fl, str, e, g, octal,
                       u, x, x, &my_s21_count, &str, num, num, fl, fl);
  // printf("s21_    input: %s; s21_count: %d; co: %d\n", s21_input, s21_count,
  // s21_co);
  co = sprintf(input, test_3_format, num, ch, fl, str, e, g, octal, u, x, x,
               &s21_count, &str, num, num, fl, fl);
  // printf("        input: %s; s21_count: %d; co: %d\n", input, s21_count, co);

  if (strcmp(s21_input, input) != 0 || co != s21_co ||
      my_s21_count != s21_count) {
    success = 0;
    printf("test_number: %d\n", test_number);
  }
  test_number++;
  // test 4
  s21_co = s21_sprintf(s21_input, test_4_format, lnum, u, my_s21_count, count,
                       u, lnum);
  // printf("s21_    input: %s; s21_count: %d; co: %d\n", s21_input, s21_count,
  //        s21_co);

  co = sprintf(input, test_4_format, lnum, u, s21_count, count, u, lnum);
  // printf("sprintf input: %s; s21_count: %d; co: %d\n", input, s21_count, co);

  if (strcmp(s21_input, input) != 0 || co != s21_co ||
      s21_count != my_s21_count) {
    success = 0;
    printf("test_number: %d\n", test_number);
  }
  test_number++;
  // test 5
  s21_co = s21_sprintf(s21_input, test_5_format, num, num, num, num, 2, num,
                       num, fl);
  // printf("s21_input: %s; co: %d\n", s21_input, s21_co);
  co = sprintf(input, test_5_format, num, num, num, num, 2, num, num, fl);
  // printf("    input: %s; co: %d\n", input, co);

  if (strcmp(s21_input, input) != 0 || co != s21_co) {
    printf("test_number: %d\n", test_number);
    success = 0;
  }
  test_number++;
  if (success) printf("success\n");

  return 0;
}