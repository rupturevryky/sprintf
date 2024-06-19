int repeated_error(char ch) {
  fprintf(stderr, "\033[31;1merror:\033[0m repeated '%c' flag in format\n", ch);
  return 1;
}

int length_modifier_error(char len_flag, char f) {
  fprintf(stderr,
          "\033[31;1merror:\033[0m use of ‘%c’ length modifier with ‘%c’ "
          "type character has either no effect or undefined behavior "
          "[\033[31;1m-Werror=format=\033[0m]\n",
          len_flag, f);
  return 1;
}

int argument_is_of_type_error(char* format, char specifer) {
  fprintf(stderr,
          "\033[31;1merror:\033[0m format ‘%%%c’ expects argument of type "
          "‘%s’ but the argument is of type wider than "
          "‘%s’\n",
          specifer, format, format);
  return 1;
}

int directive_writing_more_bytes_error() {
  fprintf(stderr,
          "\033[31;1merror:\033[0m ‘%%o’ directive writing 11 bytes into a "
          "region of size 10\n");
  return 1;
}

void gnu_printf_format_error(char flag, char ch) {
  fprintf(stderr,
          "\033[31;1merror:\033[0m '%c' flag used with ‘%%%c’ gnu_printf "
          "format [\033[31;1m-Werror=format=\033[0m]\n",
          flag, ch);
}

int check_errors(char ch, Flags* flags) {
  int res = 0;
  if (ch == 'c' || ch == 's' || ch == 'p' || ch == 'n' || ch == 'o' ||
      ch == 'u' || ch == 'x' || ch == 'X') {
    if (flags->space) {
      gnu_printf_format_error(' ', ch);
      res = 1;
    }
    if (flags->need_pluse) {
      gnu_printf_format_error('+', ch);
      res = 1;
    }
    if ((ch == 'c' || ch == 'p') && flags->zero) {
      gnu_printf_format_error('0', ch);
      res = 1;
    }
  }
  if (ch == '%' && flags->space) {
    fprintf(stderr,
            "\033[31;1merror:\033[0m conversion lacks type at end of format "
            "[\033[31;1m-Werror=format=\033[0m]\n");
    res = 1;
  }
  if (ch == 'n' && flags->zero) {
    gnu_printf_format_error('0', ch);
    res = 1;
  }

  return res;
}