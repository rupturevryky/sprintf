void take_len(const char **f, Flags *flags) {
  if (**f == '+') {
    flags->need_pluse = 1;
    *f += 1;
  }
  char num[10];
  int i = 0;
  while (**f > 47 && **f < 58) {
    num[i] = **f;
    i++;
    *f += 1;
  }
  num[i] = '\0';
  flags->offset = str_to_int(num);
}

int take_flags(const char **f, Flags *flags, int res) {
  switch (**f) {
    case '-':
      if (flags->minus == 1) {
        fprintf(stderr,
                "\033[31;1merror:\033[0m repeated '-' flag in format\n");
        return 1;
      }
      *f += 1;
      take_len(f, flags);
      flags->minus = 1;
      res = take_flags(f, flags, res);
      break;
    case '+':
      if (flags->need_pluse == 1) {
        fprintf(stderr,
                "\033[31;1merror:\033[0m repeated '+' flag in format\n");
        return 1;
      }
      take_len(f, flags);
      res = take_flags(f, flags, res);
      break;
    case ' ':
      if (flags->space == 1) {
        fprintf(stderr,
                "\033[31;1merror:\033[0m repeated ' ' flag in format\n");
        return 1;
      }
      flags->space = 1;
      *f += 1;
      res = take_flags(f, flags, res);
      break;
    default:
      break;
  }
  return res;
}

int check_h_limits(long long int num, Flags *flags, char specifer) {
  if (specifer == 'i' || specifer == 'd') {
    if (flags->h && (num > 32767 || num < -32768)) {
      fprintf(
          stderr,
          "\033[31;1merror:\033[0m format ‘%%%c’ expects argument of type "
          "‘short int’ but the argument is of type wider than ‘short int’\n",
          specifer);
      return 1;
    }
  } else if (num < 0 || (num > 65535 && !flags->l)) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m format ‘%%%c’ expects argument of type "
            "‘unsigned short int’ but the argument is of type wider than "
            "‘unsigned short int’\n",
            specifer);
    return 1;
  }
  return 0;
}

int check_l_for_d_i(long long int num, int flag_l, char specifer) {
  if (flag_l && (num > LONG_MAX || num < LONG_MIN)) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m format ‘%%%c’ expects argument of type "
            "‘long int’ but the argument is of type wider than ‘long long "
            "int’\n",
            specifer);
    return 1;
  }
  return 0;
}

int check_l_eq_one_for_unsigned(long long int num, int flag_l, char specifer) {
  if (flag_l && ((unsigned long int)num > ULONG_MAX || num < 0)) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m format ‘%%%c’ expects argument of type "
            "‘long int’ but the argument is of type wider than ‘long long "
            "int’\n",
            specifer);
    return 1;
  }
  return 0;
}

int check_l_eq_two_for_unsigned(long long int num, int flag_l, char specifer) {
  if (flag_l && ((unsigned long long int)num > ULLONG_MAX || num < 0)) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m format ‘%%%c’ expects argument of type "
            "‘long int’ but the argument is of type wider than ‘long long "
            "int’\n",
            specifer);
    return 1;
  }
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
      (num < -2147483647 || num > 2147483647) && flags->l == 0 &&
      flags->h == 0) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m format ‘%%%c’ expects argument of type "
            "‘int’, but argument has type ‘long int’  "
            "[\033[31;1m-Werror=format=\033[0m]\n",
            specifer);
    return 1;
  }
  return 0;
}