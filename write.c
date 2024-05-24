
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

// read_float() {
//   if (count_fractional_zero > 3) {
//     for (int i = 0; i <= count_fractional_zero; i++) {
//       if (i == 1) add_char(s, '.', count);
//       add_char(s, '0', count);
//     }
//     if (count_fractional_zero == 4) {
//       float_ptr *= 10000;
//       gcvt(float_ptr, 2, buffer);
//       add_char(s, buffer[2], count);
//       add_char(s, buffer[3], count);
//     }
//     if (count_fractional_zero == 5) {
//       float_ptr *= 100000;
//       gcvt(float_ptr, 1, buffer);
//       add_char(s, buffer[2], count);
//     }

//   } else {
//     int len_int = take_len_of_int((int)float_ptr, 5);
//     if (getFirstDigit((int)float_ptr != 0)) len_int++;
//     gcvt(float_ptr, 6 + len_int - count_fractional_zero, buffer);

//     int i = 0, count_fractional = 0, wasnt_point = 1;
//     for (; buffer[i] != '\0' && buffer[i] != '\n'; i++) {
//       if (buffer[i] == '.') wasnt_point = 0;
//       add_char(s, buffer[i], count);
//       if (!wasnt_point && buffer[i] != '.') count_fractional++;
//     }
//     if (wasnt_point) add_char(s, '.', count);
//     for (; count_fractional < 6; count_fractional++) add_char(s, '0', count);
//   }
// }

void write_float(char **s, va_list *args, int *count) {
  double float_ptr = va_arg(*args, double);

  int count_fractional_zero = take_zero_count(s, count, &float_ptr, 0);
  char buffer[25];

  // char buffer = read_float();

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

char *take_e_part(int e, char E) {
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

char rounding(char num, char next_num) {
  // printf("rounding: %c, %c\n", num, next_num);
  int num1 = 0;
  if (num != 'l') num1 = num - '0';
  int num2 = next_num - '0';

  if (num2 > 5) num1++;

  // printf("rounding: %c\n", num1 + '0');
  if (num1 == 10) return 'l';
  return num1 + '0';
}

void rounding_all_fractional(char *float_arr, int *count_zero,
                             int *real_count_zero) {
  // printf("float_arr: %s\n", float_arr);
  // printf("sizeof(float_arr): %d\n", (int)strlen(float_arr));
  // printf("float_arr[%d]: %c\n", (int)strlen(float_arr) - 1,
  //        float_arr[strlen(float_arr) - 1]);

  // printf("(int)strlen(buffer) - 1: %d\n", (int)strlen(float_arr) - 1);
  // printf("6 + tmp_count_zero: %d\n", 6 + *count_zero);

  int number_boundary = 5;
  if (float_arr[0] == '0') number_boundary += *count_zero;
  char now_char = '0';
  int i = strlen(float_arr) - 2;
  for (; i > number_boundary; i--) {
    now_char = rounding(float_arr[i], float_arr[i + 1]);
    if (float_arr[i + 1] == 'l') float_arr[i + 1] = '0';

    if (now_char != 'l') {
      float_arr[i] = now_char;
      // printf("now_char != 'l'; float_arr[%d]: %c\n", i, float_arr[i]);
    } else {
      float_arr[i] = '0';
      // printf("l case:   ");
      float_arr[i - 1] = rounding(float_arr[i - 1], '9');
      // printf("float_arr[%d]: %c\n", i, float_arr[i]);
      // printf("float_arr[%d]: %c\n", i - 1, float_arr[i - 1]);
      i--;
    }
  }
  i++;
  int tmp_first_char = float_arr[0];
  // printf("float_arr[%d]: %c\n", i, float_arr[i]);
  while (i > 0 && float_arr[i] == 'l') {
    float_arr[i] = '0';
    float_arr[i - 1] = rounding(float_arr[i - 1], '9');
    i--;
  }
  if ((tmp_first_char == '9' || tmp_first_char == '0') &&
      tmp_first_char != float_arr[0]) {
    // *count_zero -= 1;
    *real_count_zero -= 1;
  }
  if (float_arr[0] == 'l') {
    float_arr[0] = '0';
    memmove(float_arr + 1, float_arr, strlen(float_arr) - 1);
    float_arr[0] = '1';
  }
  // for (; i > 0; i--) printf("float_arr[%d]: %c\n", i, float_arr[i]);
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
  arr[i - 1] = '\0';
}

void write_ready_scientific_num(char **s, int *count, char *buffer,
                                int count_zero, double float_ptr, char E) {
  // printf("count_zero: %d\n", count_zero);
  int tmp_count_zero = count_zero;
  if ((int)strlen(buffer) - 1 == 8 && 6 + count_zero >= 11) tmp_count_zero = 0;

  int number_boundary = 7;
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
  // printf("start: %d\n", start);
  for (; start < 7; start++) add_char(s, '0', count);

  int e = 0;
  if ((int)float_ptr > 0) {
    e = take_len_of_int((int)float_ptr, 5);
    if (getFirstDigit((int)float_ptr) == 9 && buffer[0] == '1') e = 1;
  } else
    e = 0 - count_zero;

  // printf("(int)float_ptr: %d\n", (int)float_ptr);
  // printf("buffer: %s\n", buffer);

  // printf("E: %d\n", e);
  // printf("need_zero: %d\n", need_zero);

  char *e_buffer = take_e_part(e, E);
  // printf("e_buffer: %s\n", e_buffer);

  for (int i = 0; e_buffer[i] != '\0'; i++) add_char(s, e_buffer[i], count);
}

int write_scientific_notation(char **s, va_list *args, int *count, char E) {
  double float_ptr = va_arg(*args, double);

  char buffer[17];
  gcvt(float_ptr, 17, buffer);

  int count_zero = take_zero_count(s, count, &float_ptr, 1);
  if ((int)float_ptr == 0) count_zero++;

  // printf("count_zero after: %d\n", count_zero);
  // printf("buffer: %s\n", buffer);
  delate_point(buffer);
  int tmp_count_zero = count_zero;
  // printf("(int)strlen(buffer) - 1: %d\n", (int)strlen(buffer) - 1);
  // printf("6 + tmp_count_zero: %d\n", 6 + count_zero);

  if ((int)strlen(buffer) - 1 == 8 && 6 + count_zero >= 11) tmp_count_zero = 0;
  if ((int)strlen(buffer) - 1 > 6 + tmp_count_zero)
    rounding_all_fractional(buffer, &tmp_count_zero, &count_zero);
  // printf("buffer after: %s\n", buffer);

  write_ready_scientific_num(s, count, buffer, count_zero, float_ptr, E);

  return 0;
}

int write_string(char **s, va_list *args, int *count) {
  char *str_param = va_arg(*args, char *);
  for (int i = 0; str_param[i] != '\0'; i++) {
    add_char(s, str_param[i], count);
  }
  return 0;
}