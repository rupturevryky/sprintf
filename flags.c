#include "sprintf.h"

int flag_zero(const char **f, Flags *flags) {
  if (flags->zero == 1) return repeated_error(**f);
  *f += 1;
  flags->zero = 1;
  return 0;
}

int take_len(const char **f, Flags *flags, int without_check) {
  if (**f == 48 && !without_check && flag_zero(f, flags)) return 1;

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
  return str_to_int(num);
}

int take_flags(const char **f, Flags *flags, int res, va_list *args) {
  if (!flags->offset) flags->offset = take_len(f, flags, 0);
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
      if (flag_zero(f, flags)) return 1;
      res = take_flags(f, flags, res, args);
      break;
    case '*':
      if (flags->star == 1) return repeated_error(**f);
      *f += 1;
      flags->star = 1;
      flags->offset = va_arg(*args, int);
      res = take_flags(f, flags, res, args);
      break;
    case '.':
      if (flags->point > 0) return repeated_error(**f);
      *f += 1;
      if (**f == '*') {
        *f += 1;
        flags->point = va_arg(*args, int);
      } else
        flags->point = take_len(f, flags, 1);
      // printf("take_len %d\n", flags->point);
      res = take_flags(f, flags, res, args);
      break;
    case '#':
      if (flags->grid > 0) return repeated_error(**f);
      *f += 1;
      flags->grid = 1;
      res = take_flags(f, flags, res, args);
      break;
    default:
      if (!flags->offset) flags->offset = take_len(f, flags, 0);
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
    if ((**f != 'i' && **f != 'd' && **f != 'o' && **f != 'u' && **f != 'x' &&
         **f != 'X') ||
        flags->l != 0)
      return length_modifier_error('h', **f);
    flags->h = 1;
  } else if (ch == 'l') {
    if ((**f != 'i' && **f != 'd' && **f != 'o' && **f != 'u' && **f != 'x' &&
         **f != 'X' && **f != 'c' && **f != 's' && **f != 'l') ||
        flags->h != 0)
      return length_modifier_error('l', **f);
    flags->l += 1;
  } else if (ch == 'L') {
    if ((**f != 'e' && **f != 'E' && **f != 'f' && **f != 'g' && **f != 'G') ||
        flags->l != 0 || flags->h != 0)
      return length_modifier_error('h', **f);
    flags->L += 1;
  }
  return length_description(f, flags);
}

int check_h_limits(unsigned long long num, Flags *flags, char specifer) {
  if (num > USHRT_MAX && flags->h != 0)
    return argument_is_of_type_error("unsigned short int", specifer);
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

int check_int_limits(int num, Flags *flags, char specifer) {
  if (flags->l > 0 && (num > INT_MAX || num < INT_MIN))
    return argument_is_of_type_error("long int", specifer);

  if (flags->h > 0 && (num > SHRT_MAX || num < SHRT_MIN))
    return argument_is_of_type_error("short int", specifer);

  return 0;
}

int check_l_limits(long int num, int flag_l, char specifer) {
  if (!flag_l && (num > LONG_MAX || num < LONG_MIN))
    return argument_is_of_type_error("long int", specifer);

  return 0;
}

int check_ll_limits(long long int num, int flag_l, char specifer) {
  if (!flag_l && (num > LLONG_MAX || num < LLONG_MIN))
    return argument_is_of_type_error("long int", specifer);

  return 0;
}

int check_u_limits(unsigned long long num, int flag_l, char specifer) {
  switch (flag_l) {
    case 0:
      return (num > UINT_MAX || (long long int)num < 0)
                 ? argument_is_of_type_error("unsigned int", specifer)
                 : 0;

    case 1:
      return (num > ULONG_MAX)
                 ? argument_is_of_type_error("unsigned long", specifer)
                 : 0;

    case 2:
      return (num > ULLONG_MAX)
                 ? argument_is_of_type_error("unsigned long long", specifer)
                 : 0;
    default:
      break;
  }
  return 0;
}

int check_unsigned_limits(unsigned long long num, Flags *flags, char specifer) {
  if (flags->h != 0 && check_h_limits(num, flags, specifer)) return 1;
  if (flags->l != 0 && check_u_limits(num, flags->l, specifer)) return 1;

  return 0;
}