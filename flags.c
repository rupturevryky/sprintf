#include "sprintf.h"

void take_len(const char **f, Flags *flags) {
  char num[10];
  num[0] = 0;
  num[1] = '\0';
  int i = 0;
  while (**f > 47 && **f < 58) {
    num[i] = **f;
    i++;
    *f += 1;
  }
  num[i] = '\0';
  flags->offset = str_to_int(num);
}

int take_flags(const char **f, Flags *flags, int res, va_list *args) {
  switch (**f) {
    case '-':
      if (flags->minus == 1) return repeated_error(**f);
      *f += 1;
      flags->minus = 1;
      res = take_flags(f, flags, res, args);
      break;
    case '+':
      if (flags->need_pluse == 1) return repeated_error(**f);
      *f += 1;
      flags->need_pluse = 1;
      res = take_flags(f, flags, res, args);
      break;
    case ' ':
      if (flags->space == 1) return repeated_error(**f);
      *f += 1;
      flags->space = 1;
      res = take_flags(f, flags, res, args);
      break;
    case '0':
      if (flags->zero == 1) return repeated_error(**f);
      *f += 1;
      flags->zero = 1;
      res = take_flags(f, flags, res, args);
      break;
    case '*':
      if (flags->star == 1) return repeated_error(**f);
      *f += 1;
      flags->star = 1;
      flags->offset = va_arg(*args, int);
      res = take_flags(f, flags, res, args);
      break;
    default:
      take_len(f, flags);
      break;
  }
  return res;
}

int length_description(const char **f, Flags *flags) {
  char ch = **f;
  if (ch == 'h' || ch == 'l' || ch == 'L')
    *f += 1;
  else
    return 0;

  if (ch == 'h') {
    if (**f != 'i' && **f != 'd' && **f != 'o' && **f != 'u' && **f != 'x' &&
        **f != 'X')
      return length_modifier_error('h', **f);

    flags->h = 1;
  } else if (ch == 'l') {
    if (**f != 'i' && **f != 'd' && **f != 'o' && **f != 'u' && **f != 'x' &&
        **f != 'X' && **f != 'c' && **f != 's' && **f != 'l')
      return length_modifier_error('l', **f);

    flags->l += 1;
  } else if (ch == 'L') {
  }
  return length_description(f, flags);
}

int check_h_limits(long long int num, Flags *flags, char specifer) {
  if (specifer == 'i' || specifer == 'd') {
    if (flags->h && (num > 32767 || num < -32768))
      return argument_is_of_type_error("short int", specifer);

  } else if (num < 0 || (num > 65535 && !flags->l))
    return argument_is_of_type_error("unsigned short int", specifer);

  return 0;
}

int check_l_for_d_i(long long int num, int flag_l, char specifer) {
  if (flag_l && (num > LONG_MAX || num < LONG_MIN))
    return argument_is_of_type_error("long int", specifer);

  return 0;
}

int check_l_eq_one_for_unsigned(long long int num, int flag_l, char specifer) {
  if (flag_l && ((unsigned long int)num > ULONG_MAX || num < 0))
    return argument_is_of_type_error("unsigned long int", specifer);

  return 0;
}

int check_l_eq_two_for_unsigned(long long int num, int flag_l, char specifer) {
  if (flag_l && ((unsigned long long int)num > ULLONG_MAX || num < 0))
    return argument_is_of_type_error("unsigned long long int", specifer);
  return 0;
}

int check_l_limits(long long int num, int flag_l, char specifer) {
  int res = 0;

  if (specifer == 'i' || specifer == 'd')
    res = check_l_for_d_i(num, flag_l, specifer);
  else
    res = flag_l == 1 ? check_l_eq_one_for_unsigned(num, flag_l, specifer)
                      : check_l_eq_two_for_unsigned(num, flag_l, specifer);

  return res;
}

int check_limits(long long int num, Flags *flags, char specifer) {
  if (flags->h && check_h_limits(num, flags, specifer)) return 1;
  if (flags->l && check_l_limits(num, flags->l, specifer)) return 1;

  if ((specifer == 'i' || specifer == 'd') &&
      (num < -2147483647 || num > 2147483647) && flags->l == 0 && flags->h == 0)
    return argument_is_of_type_error("int", specifer);

  return 0;
}