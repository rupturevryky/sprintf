// #include "write_dependencies.h"
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

void add_string(char **s, char *str, int *count) {
  for (int i = 0; str[i] != '\0'; i++) add_char(s, str[i], count);
}

void clear_ending_zeros(char *float_string, int len) {
  for (int i = strlen(float_string) - 1; i > len; i--) {
    if (float_string[i] == '0')
      float_string[i] = '\0';
    else
      break;
  }
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
  for (; i > 0; i--)
    if (arr[i] == '.') {
      arr[i] = '\0';
      break;
    }
}

char *take_e_part(char E, int num, int was_a_discharge_upgrade,
                  int count_zero) {
  int e = 0;
  if (num > 0) {
    e = take_len_of_int(num, 5);
    if (was_a_discharge_upgrade) e += 1;
  } else
    e = 0 - count_zero;

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

void write_ready_scientific_num(char **s, int *count, char *buffer,
                                int count_zero, double float_ptr, char E,
                                int len) {
  int tmp_count_zero = count_zero;
  if ((int)strlen(buffer) - 1 == 8 && 6 + count_zero >= 11) tmp_count_zero = 0;

  int number_boundary = 7;
  if (len) number_boundary = len + 1;
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
  int was_a_discharge_upgrade = 0;
  if (buffer[0] == '1' && getFirstDigit((int)float_ptr) == 9) {
    was_a_discharge_upgrade = 1;
    tmp_count_zero++;
  }
  for (; start < number_boundary - tmp_count_zero; start++)
    add_char(s, '0', count);

  char *e_buffer =
      take_e_part(E, (int)float_ptr, was_a_discharge_upgrade, count_zero);

  add_string(s, e_buffer, count);
}

void reverse_arr(char *arr) {
  int length = strlen(arr);
  int start = 0;
  int end = length - 1;

  while (start < end) {
    char temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;

    start++;
    end--;
  }
}