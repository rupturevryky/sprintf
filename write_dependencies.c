#include "sprintf.h"

void add_char(char **s, char ch, int *count) {
  **s = ch;
  *s += 1;
  *count += 1;
  **s = '\0';
}

void mathematical_flags(char **s, int *count, Flags *flags) {
  if (flags->need_pluse)
    add_char(s, '+', count);
  else if (flags->space)
    add_char(s, ' ', count);
}

void add_int(char **s, long long int int_ptr, int *count) {
  int first_digit = getFirstDigit(int_ptr);
  for (int i = take_len_of_int(int_ptr, 5); i > 0; i--) {
    add_char(s, first_digit + '0', count);

    if (i != 0) {
      int_ptr %= increase_discharge(i);
      first_digit = getFirstDigit(int_ptr);
      while (i > take_len_of_int(int_ptr, 5) + 1) {
        add_char(s, '0', count);
        i--;
      }
    }
  }
}

void add_string(char **s, char *str, int *count) {
  for (int i = 0; str[i] != '\0'; i++) add_char(s, str[i], count);
}

void clear_ending_zeros(char *float_string, int len) {
  for (int i = strlen(float_string) - 1; i >= len && i > 0; i--) {
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
    e = take_len_of_int(num, 5) - 1;
    if (was_a_discharge_upgrade) e += 1;
  } else
    e = 0 - count_zero;

  static char e_buffer[4];
  e_buffer[0] = E;

  e_buffer[1] = (e >= 0) ? '+' : '-';

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

  int number_boundary = (len) ? len + 1 : 7;
  if (buffer[0] == '0') number_boundary += tmp_count_zero;

  int start = 0;
  for (int i = 0; buffer[i] != '\0' && i < number_boundary; i++) {
    if (buffer[i] == '0' && !start) continue;
    add_char(s, buffer[i], count);
    if (start == 0 && ((start + 1 < number_boundary - tmp_count_zero) ||
                       ((buffer[i + 1] != '\0') && (i + 1 < number_boundary))))
      add_char(s, '.', count);
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

void add_prev_chars(const int offset, char **s, int *count, int len, char ch) {
  *s -= len;
  // Создаем временный буфер для новой строки с достаточным размером
  int insertLen = offset - len;

  int s_len = strlen(*s);
  int temp_len = s_len > offset ? s_len + insertLen : offset;

  char temp[temp_len];
  temp[temp_len] = '\0';
  // Заполняем пробелами нужное количество позиций
  for (int i = 0; i < temp_len; i++) temp[i] = ch;
  // Копируем оставшиеся символы после индекса в новый буфер
  strncpy(temp + insertLen, *s, s_len);
  // Обновляем указатель на новую строку
  strncpy(*s, temp, temp_len);
  *s += offset;
  *count += insertLen;
}

void offset_func(int count_start, Flags *flags, char **s, int *count) {
  int len = *count - count_start;
  if (flags->minus) {
    while (len < flags->offset) {
      add_char(s, ' ', count);
      len++;
    }
    if (flags->space) {
      *s -= 1;
      *count -= 1;
      add_prev_chars(flags->offset, s, count, len, ' ');
      add_char(s, ' ', count);
    }
  } else if (flags->zero && len < flags->offset)
    add_prev_chars(flags->offset, s, count, len, '0');

  else if ((flags->need_pluse || flags->space || flags->offset > 0) &&
           len < flags->offset)
    add_prev_chars(flags->offset, s, count, len, ' ');
}