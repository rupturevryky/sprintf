#include "sprintf.h"

int increase_discharge(int exp) {
  if (exp == 0) return 0;
  int base = 10;
  for (int i = 1; i < exp; i++) base *= 10;
  return base;
}

int take_len_of_int(int num, int now) {
  if (num > 1000000000) return 9;

  if (num < increase_discharge(now) && num >= increase_discharge(now - 1))
    return now - 1;

  if (num > increase_discharge(now)) return take_len_of_int(num, now + 1);
  if (num < increase_discharge(now)) return take_len_of_int(num, now - 1);

  return now;
}

int getFirstDigit(int number) {
  while (number >= 10) {
    number /= 10;
  }
  return number;
}

int digit_down(int *num, int *tpm_len, int *len, int E, int *count, char **s,
               int should_point) {
  int del = increase_discharge(take_len_of_int(*num, 5));
  if (del == 0) del = 1;

  if (E) {
    char first_digit = getFirstDigit(*num);
    add_char(s, first_digit + '0', count);
    if (should_point) add_char(s, '.', count);
  }

  *num -= del * getFirstDigit(*num);
  int now_len = take_len_of_int(*num, 5);
  int delta = (*tpm_len) - now_len;
  if (delta > 1) {
    *len -= delta - 1;
  } else
    *tpm_len = now_len;

  return delta;
}

int take_zero_count(char **s, int *count, double *float_ptr, int is_long) {
  if (*float_ptr < 0) {
    *float_ptr = 0 - *float_ptr;
    add_char(s, '-', count);
  }
  if (*float_ptr == 0.) return 6;
  int count_fractional_zero = 0;
  while (1) {
    double one = 0.1;
    for (int i = 0; i < count_fractional_zero; i++) one /= 10.;
    if (*float_ptr < one)
      count_fractional_zero++;
    else
      break;
  }

  if (count_fractional_zero > 6 && !is_long) count_fractional_zero = 6;
  return count_fractional_zero;
}

int zero_count_for_scientific(char **s, int *count, double *float_ptr,
                              char *buffer, int *tmp_count_zero) {
  int count_zero = take_zero_count(s, count, float_ptr, 1);
  if ((int)*float_ptr == 0) count_zero++;

  delate_point(buffer);

  *tmp_count_zero = count_zero;
  int str_len = (int)strlen(buffer) - 1;

  if (str_len == 8 && 6 + count_zero >= 11) *tmp_count_zero = 0;
  while (*tmp_count_zero + 6 >= str_len) *tmp_count_zero -= 6;

  return count_zero;
}

char rounding(char num, char next_num) {
  int num1 = 0;
  if (num != 'l') num1 = num - '0';
  int num2 = next_num - '0';

  if (num2 >= 5) num1++;

  if (num1 == 10) return 'l';
  return num1 + '0';
}

int rounding_all_fractional(char *float_arr, int count_zero,
                            int *real_count_zero, int len) {
  int number_boundary = len - 1;
  if (float_arr[0] == '0' && count_zero > 1) number_boundary += count_zero - 1;

  char now_char = '0';
  int i = strlen(float_arr) - 2;

  for (; i > number_boundary; i--) {
    now_char = rounding(float_arr[i], float_arr[i + 1]);
    if (float_arr[i + 1] == 'l') float_arr[i + 1] = '0';

    if (now_char != 'l') {
      float_arr[i] = now_char;
    } else {
      float_arr[i] = '0';
      float_arr[i - 1] = rounding(float_arr[i - 1], '9');
      i--;
    }
  }
  i++;
  int tmp_first_char = float_arr[0];
  while (i > 0 && float_arr[i] == 'l') {
    float_arr[i] = '0';
    float_arr[i - 1] = rounding(float_arr[i - 1], '9');
    i--;
  }
  if ((tmp_first_char == '9' || tmp_first_char == '0') &&
      tmp_first_char != float_arr[0])
    *real_count_zero -= 1;

  if (float_arr[0] == 'l') {
    float_arr[0] = '0';
    memmove(float_arr + 1, float_arr, strlen(float_arr) - 1);
    float_arr[0] = '1';
    return 1;
  }
  return 0;
}