#include "sprintf.h"

int increase_discharge(int exp) {
  if (exp == 0) return 0;
  int base = 10;
  for (int i = 1; i < exp; i++) base *= 10;
  return base;
}

int take_len_of_num(int num, int now) {
  // if (now == 2 || now < 2) printf("now: %d\n", now);

  if (num < increase_discharge(now) && num >= increase_discharge(now - 1))
    return now;

  if (num > increase_discharge(now))
    return take_len_of_num(num, now += now / 2);
  if (num < increase_discharge(now))
    return take_len_of_num(num, now -= now / 2);

  return now;
}

int getFirstDigit(int number) {
  while (number >= 10) {
    number /= 10;
  }
  return number;
}