
#include "sprintf.h"

void add_char(char **s, char ch, int *count) {
  **s = ch;
  *s += 1;
  *count += 1;
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

int write_float(char **s, va_list *args, int *count) {
  float float_ptr = va_arg(*args, double);

  if (float_ptr < 0) {
    float_ptr *= -1;
    add_char(s, '-', count);
  }
  int tmp_count = *count;
  add_int(s, (int)float_ptr, count);
  add_char(s, '.', count);
  add_int(s, fractional_to_integer(s, count, float_ptr, *count - tmp_count - 1),
          count);

  return 0;
}