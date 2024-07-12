
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
  int int_ptr = va_arg(*args, int);

  if (check_int_limits(int_ptr, flags, specifer)) return 1;
  int count_start = *count;
  if (int_ptr < 0) {
    int_ptr = 0 - int_ptr;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  if (int_ptr == INT_MIN)
    add_string(s, "2147483648", count);
  else
    add_int(s, int_ptr, count);

  offset_func(count_start, flags, s, count);
  return 0;
}

int write_long_int(char **s, va_list *args, int *count, Flags *flags,
                   char specifer) {
  long int int_ptr = va_arg(*args, long int);

  if (check_l_limits(int_ptr, flags->l, specifer)) return 1;

  int count_start = *count;
  if (int_ptr < 0) {
    int_ptr = 0 - int_ptr;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  if (int_ptr == LONG_MIN)
    add_string(s, "9223372036854775808", count);
  else
    add_int(s, int_ptr, count);

  offset_func(count_start, flags, s, count);
  return 0;
}

int write_long_long_int(char **s, va_list *args, int *count, Flags *flags,
                        char specifer) {
  long long int int_ptr = va_arg(*args, long long int);

  if (check_ll_limits(int_ptr, flags->l, specifer)) return 1;

  int count_start = *count;
  if (int_ptr < 0) {
    int_ptr = 0 - int_ptr;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  if (int_ptr == LLONG_MIN)
    add_string(s, "9223372036854775808", count);
  else
    add_int(s, int_ptr, count);

  offset_func(count_start, flags, s, count);
  return 0;
}

int write_scientific_notation(char **s, va_list *args, int *count, char E,
                              Flags *flags) {
  double float_ptr = 0;
  if (flags->L == 1) {
    float_ptr = (long double)float_ptr;
    float_ptr = va_arg(*args, long double);
  } else
    float_ptr = va_arg(*args, double);

  int count_start = *count;
  if (float_ptr < 0) {
    float_ptr = 0 - float_ptr;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);
  char buffer[17];
  gcvt(float_ptr, 17, buffer);
  int end_ints = flags->point > -1 ? flags->point : 6;
  int tmp_count_zero = 0, count_zero = zero_count_for_scientific(
                              &float_ptr, buffer, &tmp_count_zero, flags);
  int len_without_zero = 0;
  int tmp_time_to_count = 0;
  for (int i = 0; buffer[i] != '\0' && len_without_zero < end_ints; i++) {
    if (buffer[i] != '0') tmp_time_to_count = 1;
    if (tmp_time_to_count) len_without_zero++;
  }
  int shift = 0;
  while (buffer[shift] == '0') {
    shift++;
  }
  for (int i = shift; i <= (int)strlen(buffer); i++) {
    buffer[i - shift] = buffer[i];
  }
  rounding_all_fractional(buffer, tmp_count_zero, &count_zero,
                          len_without_zero);
  char *new_buffer = set_scientific_point(buffer);
  write_ready_scientific_num(s, count, new_buffer, count_zero, float_ptr, E,
                             end_ints, 0);
  offset_func(count_start, flags, s, count);

  return 0;
}

void write_float(char **s, va_list *args, int *count, Flags *flags) {
  double float_ptr = 0;
  if (flags->L == 1) {
    float_ptr = (long double)float_ptr;
    float_ptr = va_arg(*args, long double);
  } else
    float_ptr = va_arg(*args, double);
  int count_start = *count;
  if (float_ptr < 0) {
    float_ptr = 0 - float_ptr;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  int count_fractional_zero = take_zero_count(&float_ptr, 0, flags);
  int end_ints = flags->point > -1 ? flags->point : 6;

  char buffer[25];
  if (count_fractional_zero > 3) {
    int i = 0;
    for (; i <= count_fractional_zero && i <= end_ints; i++) {
      if (i == 1) add_char(s, '.', count);
      add_char(s, '0', count);
    }
    if (count_fractional_zero == 4) {
      float_ptr *= 10000;
      gcvt(float_ptr, 2, buffer);
      if (i <= end_ints) {
        add_char(s, buffer[2], count);
        i++;
      }
      if (i <= end_ints) {
        add_char(s, buffer[3], count);
        i++;
      }
    }
    if (count_fractional_zero == 5) {
      float_ptr *= 100000;
      gcvt(float_ptr, 1, buffer);
      if (i <= end_ints) add_char(s, buffer[2], count);
    }
  } else {
    int len_int = take_len_of_int((int)float_ptr, 5);
    if (getFirstDigit((int)float_ptr != 0)) len_int++;
    int str_len = end_ints + len_int - count_fractional_zero - 1;
    gcvt(float_ptr, str_len, buffer);
    add_float(end_ints, buffer, s, count);
  }
  offset_func(count_start, flags, s, count);
}

void write_shortest_representation(char **s, va_list *args, int *count,
                                   char mode, Flags *flags) {
  int count_start = *count, len = flags->point > -1 ? flags->point : 6,
      limit = len == 0 ? -1 : 0;
  double float_ptr = 0;
  if (flags->L == 1) {
    float_ptr = (long double)float_ptr;
    float_ptr = va_arg(*args, long double);
  } else
    float_ptr = va_arg(*args, double);
  if (float_ptr < 0) {
    float_ptr = 0 - float_ptr;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  char buffer[17];
  gcvt(float_ptr, 17, buffer);
  int tmp_count_zero = 0;
  int count_zero =
      zero_count_for_scientific(&float_ptr, buffer, &tmp_count_zero, flags);
  int len_without_zero = len_w_z_for_s_rep(len, limit, buffer);
  char *buffer_2 = repeat_arr_for_shortest_representation(buffer);
  int was_greade = rounding_all_fractional(buffer, tmp_count_zero, &count_zero,
                                           len_without_zero);
  if (float_ptr == 0.) buffer[0] = '0';
  int len_of_int = take_len_of_int((int)float_ptr, 5) + was_greade;
  if ((float_ptr < 0.0001 && float_ptr != 0.) ||
      sientific_if(len_of_int, len, flags->point)) {
    char *new_buffer =
        make_ready_arr(&was_greade, buffer_2, tmp_count_zero, len, len_of_int);
    write_ready_scientific_num(s, count, new_buffer, count_zero, float_ptr,
                               mode, len, 1);
  } else {
    int wasnt_point = add_shortest_representation(
        buffer, len_of_int, &count_zero, &len, &limit, s, count);
    if (len_of_int > len && flags->point != -1 && wasnt_point &&
        getFirstDigit((int)float_ptr) != 0) {
      int was_a_discharge_upgrade = 0;
      if (buffer[0] == '1' && getFirstDigit((int)float_ptr) == 9)
        was_a_discharge_upgrade = 1;
      add_string(s,
                 take_e_part(mode, (int)float_ptr, was_a_discharge_upgrade,
                             count_zero),
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
  unsigned long long decimalNumber = va_arg(*args, unsigned long long);
  if (check_unsigned_limits(decimalNumber, flags, 'o')) return 1;
  int count_start = *count;

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
  if (flags->grid) add_char(s, '0', count);
  add_string(s, octal, count);
  offset_func(count_start, flags, s, count);

  return 0;
}

int write_unsigned_int(char **s, va_list *args, int *count, Flags *flags) {
  unsigned long long uns_long = va_arg(*args, unsigned long long);
  if (check_unsigned_limits(uns_long, flags, 'u')) return 1;
  int count_start = *count;

  add_int(s, uns_long, count);
  offset_func(count_start, flags, s, count);
  return 0;
}

int write_unsigned_hexadecimal_integer(char **s, va_list *args, int *count,
                                       char mode, Flags *flags) {
  unsigned long long num = va_arg(*args, unsigned long long);
  if (check_unsigned_limits(num, flags, mode)) return 1;
  int count_start = *count;
  char *buffer = decimal_to_hex(num, mode);

  if (flags->grid && num != 0) add_char(s, mode, count);
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