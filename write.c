
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

  if (check_limits(int_ptr, flags, specifer)) return 1;

  int count_start = *count;
  if (int_ptr < 0) {
    int_ptr *= -1;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  add_int(s, int_ptr, count);

  offset_func(count_start, flags, s, count);
  return 0;
}

int write_long_int(char **s, va_list *args, int *count, Flags *flags,
                   char specifer) {
  long int int_ptr = va_arg(*args, long int);

  if (check_limits(int_ptr, flags, specifer)) return 1;

  int count_start = *count;
  if (int_ptr < 0) {
    int_ptr *= -1;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  add_int(s, int_ptr, count);

  offset_func(count_start, flags, s, count);
  return 0;
}
int write_long_long_int(char **s, va_list *args, int *count, Flags *flags,
                        char specifer) {
  long long int int_ptr = va_arg(*args, long long int);

  if (check_limits(int_ptr, flags, specifer)) return 1;

  int count_start = *count;
  if (int_ptr < 0) {
    int_ptr *= -1;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  add_int(s, int_ptr, count);

  offset_func(count_start, flags, s, count);
  return 0;
}

int write_scientific_notation(char **s, va_list *args, int *count, char E,
                              Flags *flags) {
  double float_ptr = va_arg(*args, double);
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
  // printf("flags->point %d\n", flags->point);
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
  // Сдвигаем и перезаписываем символы
  for (int i = shift; i <= (int)strlen(buffer); i++) {
    buffer[i - shift] = buffer[i];
  }

  rounding_all_fractional(buffer, tmp_count_zero, &count_zero,
                          len_without_zero);
  // printf("scientific_notatio buffer %s\n", buffer);
  char *new_buffer = set_scientific_point(buffer);
  write_ready_scientific_num(s, count, new_buffer, count_zero, float_ptr, E,
                             end_ints, 0);
  offset_func(count_start, flags, s, count);

  return 0;
}

void write_float(char **s, va_list *args, int *count, Flags *flags) {
  double float_ptr = va_arg(*args, double);
  int count_start = *count;
  if (float_ptr < 0) {
    float_ptr = 0 - float_ptr;
    add_char(s, '-', count);
    flags->below_zero = 1;
  } else
    mathematical_flags(s, count, flags);

  int count_fractional_zero = take_zero_count(&float_ptr, 0, flags);
  int end_ints = flags->point > -1 ? flags->point : 6;

  // printf("count_fractional_zero %d; end_ints %d\n", count_fractional_zero,
  //        end_ints);

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
    // printf(
    //     "float_ptr %lf; buffer: %s; str_len: %d; len_int: %d; end_ints: "
    //     "%d; count_fractional_zero %d\n",
    //     float_ptr, buffer, str_len, len_int, end_ints,
    //     count_fractional_zero);
    int limit = -1;
    int i = 0;
    for (; buffer[i] != '\0' && limit <= end_ints; i++) {
      if (limit != -1 || buffer[i] == '.') limit++;
      add_char(s, buffer[i], count);
      if (limit >= end_ints) break;
    }
    for (; limit <= end_ints - 1; limit++) {
      // printf("limit %d\n", limit);
      add_char(s, '0', count);
    }
  }
  offset_func(count_start, flags, s, count);
}

void write_shortest_representation(char **s, va_list *args, int *count,
                                   char mode, Flags *flags) {
  int count_start = *count;
  int len = flags->point > -1 ? flags->point : 6;
  int limit = len == 0 ? -1 : 0;
  double float_ptr = va_arg(*args, double);
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
  int len_without_zero = limit;
  int tmp_time_to_count = 0;
  for (int i = 0; buffer[i] != '\0' && len_without_zero < len; i++) {
    if (buffer[i] != '0') tmp_time_to_count = 1;
    if (tmp_time_to_count) len_without_zero++;
  }

  char buffer_2[17];
  int i = 0;
  for (; buffer[i] != '\0'; i++) buffer_2[i] = buffer[i];
  buffer_2[i] = '\0';
  int was_greade = rounding_all_fractional(buffer, tmp_count_zero, &count_zero,
                                           len_without_zero);

  if (float_ptr == 0.) buffer[0] = '0';
  int len_of_int = take_len_of_int((int)float_ptr, 5) + was_greade;
  // printf(
  //     "~~start buffer: %s; count_zero %d; tmp_count_zero %d;"
  //     "len_without_zero %d; len %d; float_ptr %lf: len_of_int %d:
  //     flags->point "
  //     "%d\n",
  //     buffer, count_zero, tmp_count_zero, len_without_zero, len, float_ptr,
  //     len_of_int, flags->point);
  if ((float_ptr < 0.0001 && float_ptr != 0.) ||
      (len_of_int >= len && (flags->point > 0 || len < len_of_int) &&
       len_of_int != 1)) {
    int count_zero_in_scintific = 0;
    was_greade += rounding_all_fractional(buffer_2, tmp_count_zero,
                                          &count_zero_in_scintific, len - 1);
    char *new_buffer = set_scientific_point(buffer_2);
    clear_ending_zeros(new_buffer, len, len_of_int, count_zero_in_scintific);

    // printf(
    //     "limit %d; len %d; flags->point %d; tmp_count_zero %d; count_zero "
    //     "%d; new_buffer %s; buffer %s; "
    //     "float_ptr %lf; len_of_int %d\n",
    //     limit, len, flags->point, tmp_count_zero, count_zero, new_buffer,
    //     buffer, float_ptr, len_of_int);

    write_ready_scientific_num(s, count, new_buffer, count_zero, float_ptr,
                               mode, len, 1);
  } else {
    char *new_buffer = set_point(buffer, len_of_int);
    // printf("new_buffer %s; old buffer %s; float_ptr %lf\n", new_buffer,
    // buffer,
    //        float_ptr);
    int tmp_count_zero = (new_buffer[0] - 0 == 0 && len_of_int == 1)
                             ? count_zero - 1
                             : count_zero;
    if (new_buffer[0] - 0 == 0 && len_of_int == 1) count_zero--;
    clear_ending_zeros(new_buffer, len, len_of_int, tmp_count_zero);
    // printf(
    //     "limit %d; len %d; flags->point %d; new_buffer %s; old_buffer "
    //     "%s; len_of_int % d; count_zero %d\n",
    //     limit, len, flags->point, new_buffer, buffer, len_of_int,
    //     count_zero);
    int time_to_count = 0, wasnt_point = 1;
    for (int i = 0;
         new_buffer[i] != '\0' && limit < len &&
         ((new_buffer[i] > 47 && new_buffer[i] < 58) || new_buffer[i] == '.');
         i++) {
      if (new_buffer[i] != '0' && new_buffer[i] != '.') time_to_count = 1;
      if (time_to_count && new_buffer[i] != '.') limit++;
      add_char(s, new_buffer[i], count);
      if (new_buffer[i] == '.') wasnt_point = 0;
    }
    if (len == 0) len = 1;
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
  if (flags->grid) add_char(s, '0', count);
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