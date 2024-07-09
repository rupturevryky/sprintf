#include "sprintf.h"

int write(const char **f, char **s, va_list *args, int *count) {
  Flags flags = {0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0};
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
    if (flags.l == 1) {
      if (write_long_int(s, args, count, &flags, **f)) return 1;
    } else if (flags.l == 2) {
      if (write_long_long_int(s, args, count, &flags, **f)) return 1;
    } else if (write_int(s, args, count, &flags, **f))
      return 1;
  } else if (**f == 'e' || **f == 'E') {
    write_scientific_notation(s, args, count, **f, &flags);
  } else if (**f == 'f') {
    write_float(s, args, count, &flags);
  } else if (**f == 'g' || **f == 'G') {
    write_shortest_representation(s, args, count, **f, &flags);
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
  **s = '\0';
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
      f++;
      if (write(&f, &s, &now_arg, &count) != 0) res = 1;
    } else {
      add_char(&s, *f, &count);
      f++;
    }
    // printf("%s\n", str);
  }
  va_end(now_arg);
  if (res) exit(1);
  return count;
}

int main() {
  int num = 0;
  // int num = 1;
  // int num = 12;
  // int num = 999;
  // int num = 1234;
  // int num = -1;
  // int num = 2147483648;
  // int num = -12;
  // int num = -999;
  // int num = -1234;
  // int num = -12345;
  // int num = -123456;
  // int num = -1234567;

  long int lnum = 0;
  // long int lnum = 1;
  // long int lnum = 12;
  // long int lnum = 123;
  // long int lnum = 2247483648;
  // long int lnum = 9223372036854775807;
  // long int lnum = -1;
  // long int lnum = -12;
  // long int lnum = -123;
  // long int lnum = -2247483648;
  // long int lnum = -9223372036854775807;

  // float fl = 0.12341;
  // float fl = 0.012341;
  // float fl = 0.0012341;
  // float fl = 0.0001234999999999;
  // float fl = 0.000012345;
  // float fl = 0.0000012341;
  // float fl = 1.012341;
  // float fl = 123.12345;
  // float fl = -0.12341;
  // float fl = -0.012341;
  // float fl = -0.0012341;
  // float fl = -0.0001234999999999;
  // float fl = -0.000012345;
  // float fl = -0.0000012341;
  // float fl = -1.012341;
  float fl = -123.12345;

  double e = 1.1234;
  // double e = 0.1234567890123;
  // double e = 0.12;
  // double e = 0.012;
  // double e = 0.00012;
  // double e = 99912355.122349998;
  // double e = 0.00000000000000123456789123456789;
  // double e = 9.99999998;
  // double e = 99.999999999999;
  // double e = 0.00000123456789;
  // double e = 0.000000000000123456989123456789;

  double g = 0.;
  // double g = 1.;
  // double g = 9.99999998;
  // double g = 0.12;
  // double g = 0.121234566789012334;
  // double g = 0.00000593458789;
  // double g = 0.00000500000093458789;
  // double g = 0.00012;
  // double g = 9912345.122349998;

  long int octal = 12;
  // long int octal = 123;
  // long int octal = 1234;
  // long int octal = 12345;
  // long int octal = 2247483648;
  // long int octal = 0;

  unsigned int u = 0;
  // unsigned int u = 1;
  // unsigned int u = 12;
  // unsigned int u = 123;
  // unsigned int u = 32767;

  unsigned int x = 0;
  // unsigned int x = 1;
  // unsigned int x = 123;
  // unsigned int x = 12345;
  // unsigned int x = 12345678;
  // unsigned int x = 123456789;

  int s21_count = 0;
  int my_s21_count = 0;
  int count = 1;

  char ch = 'A';
  char B = 'B';
  char str[] = "[str]";

  char s21_input[250];
  char input[250];
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
      "1:%1d; 2:|%2d|; 3:|%4d|; 4:|% 3d|; 5:|%*d|; 6:|%15d|; 7:|%014f|";
  const char *test_6_format =
      "1:|%+5.4d|; 2:|%-8.5d|; 3:|%05d|; 4:|% 3.4d|; 5:|%*.5d|; 6:|%15.4d|; "
      "7:|%.2f|; 8:|%02.2e|; 9:|%.2e|";
  const char *test_7_format =
      "1:|%+7.4g|; 2:|%+5.4G|; 3:|%-5.4g|; 4:|%+ .10g|; 5:|%.10g|; 6:|%.1g|; "
      "7:|%.0g|; 8:|%.G|;";
  const char *test_8_format =
      "1:|%.4s|; 2:|%6.5s|; 3:|%.6s|; 4:|%.e|; 5:|%.1e|; 6:|%.0e|; 7:|%e|;";
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
  printf("--------------\n");
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
  printf("--------------\n");
  // test 3
  s21_co = s21_sprintf(s21_input, test_3_format, num, ch, fl, str, e, g, octal,
                       u, x, x, &my_s21_count, &str, num, num, fl, fl);
  // printf("s21_input: %s; s21_co: %d\n", s21_input, s21_co);
  co = sprintf(input, test_3_format, num, ch, fl, str, e, g, octal, u, x, x,
               &s21_count, &str, num, num, fl, fl);
  // printf("    input: %s; co: %d\n", input, co);
  if (strcmp(s21_input, input) != 0 || co != s21_co ||
      my_s21_count != s21_count) {
    success = 0;
    printf("test_number: %d\n", test_number);
  }
  test_number++;
  printf("--------------\n");
  // test 4
  s21_co =
      s21_sprintf(s21_input, test_4_format, lnum, u, my_s21_count, count, u, x);
  // printf("s21_input: %s; s21_count: %d; co: %d\n", s21_input, s21_count,
  //  s21_co);
  co = sprintf(input, test_4_format, lnum, u, s21_count, count, u, x);
  // printf("    input: %s; co: %d\n", input, co);
  if (strcmp(s21_input, input) != 0 || co != s21_co ||
      s21_count != my_s21_count) {
    success = 0;
    printf("test_number: %d\n", test_number);
  }
  test_number++;
  printf("--------------\n");
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
  printf("--------------\n");
  // test 6
  s21_co = s21_sprintf(s21_input, test_6_format, num, num, num, num, 2, num,
                       num, fl, e, e);
  // printf("s21_input: %s; co: %d\n", s21_input, s21_co);
  co = sprintf(input, test_6_format, num, num, num, num, 2, num, num, fl, e, e);
  // printf("    input: %s; co: %d\n", input, co);
  if (strcmp(s21_input, input) != 0 || co != s21_co) {
    printf("test_number: %d\n", test_number);
    success = 0;
  }
  test_number++;
  printf("--------------\n");
  // test 7
  s21_co = s21_sprintf(s21_input, test_7_format, fl, e, g, fl, e, g, fl, e);
  // printf("s21_input: %s; co: %d\n", s21_input, s21_co);
  co = sprintf(input, test_7_format, fl, e, g, fl, e, g, fl, e);
  // printf("    input: %s; co: %d\n", input, co);
  if (strcmp(s21_input, input) != 0 || co != s21_co) {
    printf("test_number: %d\n", test_number);
    success = 0;
  }
  test_number++;
  printf("--------------\n");
  // test 8
  s21_co = s21_sprintf(s21_input, test_8_format, str, str, str, 0.1, 0.01, 0.01,
                       123.456);
  // printf("s21_input: %s; co: %d\n", s21_input, s21_co);
  co = sprintf(input, test_8_format, str, str, str, 0.1, 0.01, 0.01, 123.456);
  // printf("    input: %s; co: %d\n", input, co);
  if (strcmp(s21_input, input) != 0 || co != s21_co) {
    printf("test_number: %d\n", test_number);
    success = 0;
  }
  if (success) printf("success\n");

  return 0;
}
