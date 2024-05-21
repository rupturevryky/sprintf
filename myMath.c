#include "sprintf.h"

int increase_discharge(int exp) {
  if (exp == 0) return 0;
  int base = 10;
  for (int i = 1; i < exp; i++) base *= 10;
  return base;
}

int take_len_of_int(int num, int now) {
  // if (now == 2 || now < 2) printf("now: %d\n", now);

  if (num < increase_discharge(now) && num >= increase_discharge(now - 1))
    return now - 1;

  if (num > increase_discharge(now))
    return take_len_of_int(num, now += now / 2);
  if (num < increase_discharge(now))
    return take_len_of_int(num, now -= now / 2);

  return now;
}

int getFirstDigit(int number) {
  while (number >= 10) {
    number /= 10;
  }
  return number;
}

void add_char(char **s, char ch, int *count);

int fractional_to_integer(char **s, int *count, float number, int tmp_count) {
  // printf("number: %f, tmp_count: %d\n", number, tmp_count);
  if ((int)number == 0) number++;

  while ((int)number != number) number *= 10;

  int tmp = (int)number;
  int tpm_len = take_len_of_int(tmp, 5);
  // printf("tmp: %d\n", tmp);

  for (; tmp_count > 0; tmp_count--) {
    int del = increase_discharge(take_len_of_int(tmp, 5));
    if (del == 0) del = 1;
    tmp -= del * getFirstDigit(tmp);
    // printf("tmp: %d\n", tmp);

    int now_len = take_len_of_int(tmp, 5);
    int delta = tpm_len - now_len;
    // printf("delta: %d\n", delta);

    if (delta > 1) {
      tmp_count -= delta - 1;
    } else
      tpm_len = now_len;
    // printf("tmp_count: %d\n", tmp_count);

    if (tmp_count - 1 <= 0 && delta > 1)
      for (int i = delta - 1; i > 0; i--) add_char(s, '0', count);
    // printf("111\n");
  }
  return tmp;
}