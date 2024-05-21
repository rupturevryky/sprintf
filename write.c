#include "sprintf.h"

int write_int(char **s, va_list *args, int *count) {
  int *int_ptr = va_arg(*args, int *);

  if (!*int_ptr) return 1;

  if (*int_ptr < 0) {
    **s = '-';
    *int_ptr *= -1;
    *s += 1;
    (*count) += 1;
  }

  int len = take_len_of_num(*int_ptr, 5);
  // printf("len: %d\n", len);

  int first_digit = getFirstDigit(*int_ptr);
  for (int i = len - 1; i >= 0; i--) {
    // printf("first_digit: %d\n", first_digit);
    // printf("i: %d\n", i);

    **s = first_digit + '0';
    // printf("s: %c\n", **s);

    if (i != 0) {
      *int_ptr %= increase_discharge(i);
      first_digit = getFirstDigit(*int_ptr);
    }

    // printf("int_ptr: %d\n", *int_ptr);

    *s += 1;
  }
  // free(new_int);
  (*count) += len;

  return 0;
}

int write_char(char **s, va_list *args, int *count) {
  char *char_ptr = va_arg(*args, char *);
  if (!char_ptr) return 1;

  **s = *char_ptr;
  *s += 1;
  (*count)++;

  return 0;
}