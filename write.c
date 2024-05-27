
#include "sprintf.h"

void add_char(char **s, char ch, int *count) {
  **s = ch;
  *s += 1;
  *count += 1;
}

void add_string(char **s, char *str, int *count) {
  for (int i = 0; str[i] != '\0'; i++) add_char(s, str[i], count);
}

void add_int(char **s, int int_ptr, int *count) {
  int first_digit = getFirstDigit(int_ptr);
  for (int i = take_len_of_int(int_ptr, 5); i >= 0; i--) {
    add_char(s, first_digit + '0', count);

    if (i != 0) {
      int_ptr %= increase_discharge(i);
      first_digit = getFirstDigit(int_ptr);
    }
  }
}

int write_int(char **s, va_list *args, int *count) {
  int int_ptr = va_arg(*args, int);

  if (int_ptr < 0) {
    int_ptr *= -1;
    add_char(s, '-', count);
  }
  add_int(s, int_ptr, count);

  return 0;
}

int write_char(char **s, va_list *args, int *count) {
  char char_ptr = va_arg(*args, int);
  if (!char_ptr) return 1;

  add_char(s, char_ptr, count);

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

char *take_e_part(char E, int num, int was_a_discharge_upgrade,
                  int count_zero) {
  int e = 0;
  if (num > 0) {
    e = take_len_of_int(num, 5);
    if (was_a_discharge_upgrade) e += 1;
  } else
    e = 0 - count_zero;

  static char e_buffer[4];
  e_buffer[0] = E;
  if (e >= 0)
    e_buffer[1] = '+';
  else
    e_buffer[1] = '-';
  if (e < 0) e = 0 - e;
  if (e < 10) {
    e_buffer[2] = '0';
    e_buffer[3] = e + '0';
  } else {
    int first_digit = getFirstDigit(e);
    char digit = first_digit + '0';
    e_buffer[2] = digit;
    e_buffer[3] = e % 10 + '0';
  }
  e_buffer[4] = '\0';
  return e_buffer;
}

void delate_point(char *arr) {
  int i = 0;
  for (; arr[i] != '\0' && arr[i] != '\n'; i++) {
    if (arr[i + 1] < 46 || arr[i + 1] > 57) {
      i++;
      break;
    }
    if (arr[i] == '.' && arr[i + 1] != '\0') {
      arr[i] = arr[i + 1];
      arr[i + 1] = '.';
    }
  }
  for (; i > 0; i--)
    if (arr[i] == '.') {
      arr[i] = '\0';
      break;
    }
}

void write_ready_scientific_num(char **s, int *count, char *buffer,
                                int count_zero, double float_ptr, char E,
                                int len) {
  int tmp_count_zero = count_zero;
  if ((int)strlen(buffer) - 1 == 8 && 6 + count_zero >= 11) tmp_count_zero = 0;

  int number_boundary = 7;
  if (len) number_boundary = len + 1;
  if (buffer[0] == '0') number_boundary += tmp_count_zero;

  int start = 0;
  for (int i = 0; buffer[i] != '\0' && i < number_boundary; i++) {
    if (buffer[i] == '0' && !start) continue;
    add_char(s, buffer[i], count);
    if (start == 0) add_char(s, '.', count);
    start++;
  }
  if (start == 0) {
    add_char(s, '0', count);
    add_char(s, '.', count);
    start++;
  }
  int was_a_discharge_upgrade = 0;
  if (buffer[0] == '1' && getFirstDigit((int)float_ptr) == 9) {
    was_a_discharge_upgrade = 1;
    tmp_count_zero++;
  }
  for (; start < number_boundary - tmp_count_zero; start++)
    add_char(s, '0', count);

  char *e_buffer =
      take_e_part(E, (int)float_ptr, was_a_discharge_upgrade, count_zero);

  add_string(s, e_buffer, count);
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

void clear_ending_zeros(char *float_string, int len) {
  for (int i = strlen(float_string) - 1; i > len; i--) {
    if (float_string[i] == '0')
      float_string[i] = '\0';
    else
      break;
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
    add_char(s, '0', count);
    return 0;
  }

  char octal[100];
  int i = 0;
  while (decimalNumber != 0) {
    octal[i] = decimalNumber % 8 + '0';
    decimalNumber = decimalNumber / 8;
    i++;
  }

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