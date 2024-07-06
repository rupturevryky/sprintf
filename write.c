
#include "sprintf.h"
#include "write_dependencies.c"

int write_char(char **s, va_list *args, int *count, Flags *flags) {
  char char_ptr = va_arg(*args, int);
  if (!char_ptr) return 1;
  int count_start = *count;

  add_char(s, char_ptr, count);
  offset_func(count_start, flags, s, count);

  return 0;
}

int write_int(char **s, va_list *args, int *count, Flags *flags,
              char specifer) {
  long long int int_ptr = va_arg(*args, long long int);
  if (check_limits(int_ptr, flags, specifer)) return 1;

  int count_start = *count;
  if (int_ptr < 0) {
    int_ptr *= -1;
    add_char(s, '-', count);
  } else
    mathematical_flags(s, count, flags);

  add_int(s, int_ptr, count);

  offset_func(count_start, flags, s, count);
  return 0;
}

int write_scientific_notation(char **s, va_list *args, int *count, char E,
                              Flags *flags) {
  double float_ptr = va_arg(*args, double);
  char buffer[17];
  gcvt(float_ptr, 17, buffer);
  int end_ints = flags->point > -1 ? flags->point : 6;

  int count_start = *count, tmp_count_zero = 0,
      count_zero = zero_count_for_scientific(s, count, &float_ptr, buffer,
                                             &tmp_count_zero, flags);
  rounding_all_fractional(buffer, tmp_count_zero, &count_zero, end_ints);
  write_ready_scientific_num(s, count, buffer, count_zero, float_ptr, E,
                             end_ints);
  offset_func(count_start, flags, s, count);

  return 0;
}

void write_float(char **s, va_list *args, int *count, Flags *flags) {
  double float_ptr = va_arg(*args, double);
  int count_start = *count;

  int count_fractional_zero = take_zero_count(s, count, &float_ptr, 0, flags);
  int end_ints = flags->point > -1 ? flags->point : 6;

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
    int str_len = end_ints + len_int - count_fractional_zero;
    gcvt(float_ptr, str_len, buffer);

    int i = 0, count_fractional = 0, wasnt_point = 1;
    for (; buffer[i] != '\0' && buffer[i] != '\n' && i <= 1 + end_ints; i++) {
      if (buffer[i] == '.') wasnt_point = 0;
      add_char(s, buffer[i], count);
      if (!wasnt_point && buffer[i] != '.') count_fractional++;
    }
    if (wasnt_point) add_char(s, '.', count);
    for (; count_fractional < end_ints; count_fractional++)
      add_char(s, '0', count);
  }
  offset_func(count_start, flags, s, count);
}

void write_shortest_representation(char **s, va_list *args, int *count,
                                   Flags *flags) {
  int len = flags->point > 5 ? flags->point : 5;
  int limit = 0;
  double float_ptr = va_arg(*args, double);
  int count_start = *count;

  char buffer[17];
  gcvt(float_ptr, 17, buffer);

  int tmp_count_zero = 0;
  int count_zero = zero_count_for_scientific(s, count, &float_ptr, buffer,
                                             &tmp_count_zero, flags);

  int was_greade =
      rounding_all_fractional(buffer, tmp_count_zero, &count_zero, len);
  if (float_ptr == 0.) buffer[0] = '0';
  buffer[len + 1] = '\0';

  int len_of_int = take_len_of_int((int)float_ptr, 5) + was_greade;
  clear_ending_zeros(buffer, len_of_int);
  if ((float_ptr < 0.0001 && float_ptr != 0.) ||
      (len_of_int >= flags->point && flags->point > 1)) {
    write_ready_scientific_num(s, count, buffer, count_zero, float_ptr, 'e',
                               len);
  } else {
    for (int i = 0; i < 10 && buffer[i] != '\0' &&
                    (flags->point == -1 || limit < flags->point);
         i++) {
      if (buffer[i] < 58 && buffer[i] > 48) limit++;
      if (buffer[i] == 48 && limit > 0) limit++;
      if ((len_of_int == 0 && i == len_of_int + 1) ||
          (i == len_of_int && len_of_int != 0))
        add_char(s, '.', count);
      add_char(s, buffer[i], count);
    }
    if (len_of_int > flags->point && flags->point != -1) {
      int was_a_discharge_upgrade = 0;
      if (buffer[0] == '1' && getFirstDigit((int)float_ptr) == 9)
        was_a_discharge_upgrade = 1;
      add_string(
          s,
          take_e_part('e', (int)float_ptr, was_a_discharge_upgrade, count_zero),
          count);
    }
  }
  flags->point = -1;
  offset_func(count_start, flags, s, count);
}

int write_string(char **s, va_list *args, int *count, Flags *flags) {
  char *str_param = va_arg(*args, char *);
  int count_start = *count;
  if (flags->point > -1) str_param[flags->point] = '\0';
  add_string(s, str_param, count);
  flags->point = -1;
  offset_func(count_start, flags, s, count);
  return 0;
}

int write_unsigned_octal(char **s, va_list *args, int *count, Flags *flags) {
  unsigned long decimalNumber = va_arg(*args, unsigned long);
  if (check_limits(decimalNumber, flags, 'o')) return 1;
  int count_start = *count;

  if ((long long int)decimalNumber < 0)
    return directive_writing_more_bytes_error();

  if (decimalNumber == 0) {
    char ch = '0';
    add_char(s, ch, count);
    offset_func(count_start, flags, s, count);
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

  reverse_arr(octal);
  add_string(s, octal, count);
  offset_func(count_start, flags, s, count);

  return 0;
}

int write_unsigned_int(char **s, va_list *args, int *count, Flags *flags) {
  unsigned long uns_long = va_arg(*args, unsigned long);
  if (check_limits(uns_long, flags, 'u')) return 1;
  int count_start = *count;

  if (uns_long > 2147483647)
    return argument_is_of_type_error("unsigned int", 'u');

  add_int(s, uns_long, count);
  offset_func(count_start, flags, s, count);
  return 0;
}

int write_unsigned_hexadecimal_integer(char **s, va_list *args, int *count,
                                       char mode, Flags *flags) {
  unsigned long num = va_arg(*args, unsigned long);
  if (check_limits(num, flags, mode)) return 1;
  int count_start = *count;
  char *buffer = decimal_to_hex(num, mode);

  add_string(s, buffer, count);
  offset_func(count_start, flags, s, count);
  return 0;
}

void write_ptr_adress(char **s, va_list *args, int *count, Flags *flags) {
  void *ptr = va_arg(*args, void *);
  int count_start = *count;
  unsigned long long address = (unsigned long long)ptr;
  int j = 0;
  unsigned long long temp;
  char buffer[15];
  do {
    temp = address % 16;
    buffer[j++] = (temp < 10) ? (temp + '0') : (temp - 10 + 'a');
    address /= 16;
  } while (address != 0);
  buffer[j++] = 'x';
  buffer[j++] = '0';
  buffer[j] = '\0';
  reverse_arr(buffer);

  add_string(s, buffer, count);
  offset_func(count_start, flags, s, count);
}

void write_count(va_list *args, int *count) {
  int *num = va_arg(*args, int *);
  *num = *count;
}

void write_percentage(char **s, char ch, int *count, Flags *flags) {
  int count_start = *count;
  add_char(s, ch, count);
  offset_func(count_start, flags, s, count);
}