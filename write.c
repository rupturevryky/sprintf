
#include "sprintf.h"
#include "write_dependencies.c"

int write_char(char **s, va_list *args, int *count) {
  char char_ptr = va_arg(*args, int);
  if (!char_ptr) return 1;

  add_char(s, char_ptr, count);

  return 0;
}

int write_int(char **s, va_list *args, int *count) {
  int int_ptr = va_arg(*args, int);

  if (int_ptr < 0) {
    int_ptr *= -1;
    add_char(s, '-', count);
  }

  if (int_ptr < -2147483647 || int_ptr > 2147483647) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m format ‘%%d’ expects argument of type "
            "‘int’, but argument 3 has type ‘long int’  "
            "[\033[31;1m-Werror=format=\033[0m]\n");
    return 1;
  }

  add_int(s, int_ptr, count);

  return 0;
}

int write_scientific_notation(char **s, va_list *args, int *count, char E) {
  double float_ptr = va_arg(*args, double);
  char buffer[17];
  gcvt(float_ptr, 17, buffer);

  int tmp_count_zero = 0;
  int count_zero =
      zero_count_for_scientific(s, count, &float_ptr, buffer, &tmp_count_zero);

  rounding_all_fractional(buffer, tmp_count_zero, &count_zero, 6);
  write_ready_scientific_num(s, count, buffer, count_zero, float_ptr, E, 6);

  return 0;
}

void write_float(char **s, va_list *args, int *count) {
  double float_ptr = va_arg(*args, double);

  int count_fractional_zero = take_zero_count(s, count, &float_ptr, 0);
  char buffer[25];

  if (count_fractional_zero > 3) {
    for (int i = 0; i <= count_fractional_zero; i++) {
      if (i == 1) add_char(s, '.', count);
      add_char(s, '0', count);
    }
    if (count_fractional_zero == 4) {
      float_ptr *= 10000;
      gcvt(float_ptr, 2, buffer);
      add_char(s, buffer[2], count);
      add_char(s, buffer[3], count);
    }
    if (count_fractional_zero == 5) {
      float_ptr *= 100000;
      gcvt(float_ptr, 1, buffer);
      add_char(s, buffer[2], count);
    }
  } else {
    int len_int = take_len_of_int((int)float_ptr, 5);
    if (getFirstDigit((int)float_ptr != 0)) len_int++;
    gcvt(float_ptr, 6 + len_int - count_fractional_zero, buffer);

    int i = 0, count_fractional = 0, wasnt_point = 1;
    for (; buffer[i] != '\0' && buffer[i] != '\n'; i++) {
      if (buffer[i] == '.') wasnt_point = 0;
      add_char(s, buffer[i], count);
      if (!wasnt_point && buffer[i] != '.') count_fractional++;
    }
    if (wasnt_point) add_char(s, '.', count);
    for (; count_fractional < 6; count_fractional++) add_char(s, '0', count);
  }
}

void write_shortest_representation(char **s, va_list *args, int *count) {
  int len = 5;
  double float_ptr = va_arg(*args, double);

  char buffer[17];
  gcvt(float_ptr, 17, buffer);

  int tmp_count_zero = 0;
  int count_zero =
      zero_count_for_scientific(s, count, &float_ptr, buffer, &tmp_count_zero);

  int was_greade =
      rounding_all_fractional(buffer, tmp_count_zero, &count_zero, len);
  if (float_ptr == 0.) buffer[0] = '0';
  buffer[len + 1] = '\0';

  int len_of_int = take_len_of_int((int)float_ptr, 5) + was_greade;
  clear_ending_zeros(buffer, len_of_int);

  if ((float_ptr < 0.0001 && float_ptr != 0.) || len_of_int >= 6) {
    write_ready_scientific_num(s, count, buffer, count_zero, float_ptr, 'e',
                               len);
  } else {
    for (int i = 0; i < 10 && buffer[i] != '\0'; i++) {
      if (i == len_of_int + 1) add_char(s, '.', count);
      add_char(s, buffer[i], count);
    }
  }
};

int write_string(char **s, va_list *args, int *count) {
  char *str_param = va_arg(*args, char *);
  add_string(s, str_param, count);
  return 0;
}

int write_unsigned_octal(char **s, va_list *args, int *count) {
  unsigned long decimalNumber = va_arg(*args, unsigned long);

  if ((long long int)decimalNumber < 0) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m ‘%%o’ directive writing 11 bytes into a "
            "region of size 10\n");
    return 1;
  }
  if (decimalNumber == 0) {
    char ch = '0';
    add_char(s, ch, count);
    return 0;
  }

  int i = 0;
  char octal[100];
  while (decimalNumber != 0) {
    octal[i] = decimalNumber % 8 + '0';
    decimalNumber = decimalNumber / 8;
    i++;
  }
  octal[i] = '\0';

  int length = strlen(octal);
  int start = 0;
  int end = length - 1;

  while (start < end) {
    char temp = octal[start];
    octal[start] = octal[end];
    octal[end] = temp;

    start++;
    end--;
  }
  add_string(s, octal, count);

  return 0;
}

int write_unsigned_int(char **s, va_list *args, int *count) {
  unsigned long int_ptr = va_arg(*args, unsigned long);

  if (int_ptr > 2147483647) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m format ‘%%u’ expects argument of type "
            "‘unsigned int’, but "
            "argument 3 has other type\n");
    return 1;
  }
  add_int(s, int_ptr, count);
  return 0;
}