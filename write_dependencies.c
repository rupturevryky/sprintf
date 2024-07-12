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

void add_int(char **s, unsigned long long int_ptr, int *count) {
  int first_digit = getFirstDigit(int_ptr);
  int i = take_len_of_int(int_ptr, 5);
  for (; i > 0; i--) {
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

void clear_ending_zeros(char *float_string, int len, int int_len,
                        int prev_zero_count) {
  if (len == 0) len = 1;

  int i = strlen(float_string) - 1;
  for (; i >= len && i > 0; i--) {
    if (float_string[i] == '0')
      float_string[i] = '\0';
    else
      break;
  }

  i = int_len + len + prev_zero_count;
  for (; i >= int_len && i > 0; i--) {
    if (float_string[i] == '0') {
      float_string[i] = '\0';
      if (float_string[i - 1] == '.')
        float_string[i - 1] = '\0';
      else
        break;
    } else
      break;
  }
  int point_index = -1, start = -1, zero_count = 0;
  for (int j = 0; float_string[j] != '\0'; j++) {
    if (float_string[j] == '.') point_index = j;
    if (start == -1 && float_string[j] > 48 && float_string[j] < 58) start = j;
    if (start != -1 && zero_count != -1 && point_index > 0 &&
        float_string[j] == '0') {
      zero_count++;
      if (zero_count + int_len >= len) break;
    } else if (zero_count > 0 ||
               (point_index > 0 && start != -1 && float_string[j] != '.'))
      zero_count = -1;
  }
  if (zero_count > 4) float_string[point_index] = '\0';
  for (int j = 0; float_string[j] != '\0'; j++) i = j;
  while ((float_string[i] == '0' || float_string[i] == '.') &&
         i >= point_index && i > 0) {
    float_string[i] = '\0';
    i--;
  }

  i = prev_zero_count + len;
  for (; i >= int_len && i > 0; i--) {
    if (float_string[i] == '0')
      float_string[i] = '\0';
    else
      break;
  }
}
void delate_point(char *arr) {
  if (arr[0] == '-') {
    printf("minus\n");
    int length = strlen(arr);
    for (int i = 0; i < length - 1; i++) {
      arr[i] = arr[i + 1];
    }
    arr[length - 1] = '\0';
  }

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
  if (E == 'G') E = 'E';
  if (E == 'g') E = 'e';
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

char *set_point(char *float_arr, int len_of_int) {
  int length = strlen(float_arr);
  static char new_str[50];
  for (int i = 0, j = 0; i < length; i++, j++) {
    if (i == len_of_int) {
      new_str[j] = '.';
      j++;
    }
    new_str[j] = float_arr[i];
    new_str[j + 1] = '\0';
  }
  return new_str;
}

char *set_scientific_point(char *scientific_num) {
  int i = 0, j = 0;
  int was_other_int = 0;
  static char tmp[20];
  for (; scientific_num[i] != '\0'; i++) {
    if (scientific_num[i] == '0' && !was_other_int) continue;
    was_other_int++;
    tmp[j] = scientific_num[i];
    j++;
    if (was_other_int == 1) {
      tmp[j] = '.';
      j++;
    }
  }
  tmp[j] = '\0';
  return tmp;
}

void write_ready_scientific_num(char **s, int *count, char *buffer,
                                int count_zero, double float_ptr, char E,
                                int len, int G_mode) {
  if (buffer[0] == 'i') {
    add_string(s, "inf", count);
    return;
  }
  int tmp_count_zero = count_zero;
  if ((int)strlen(buffer) - 1 == 8 && 6 + count_zero >= 11) tmp_count_zero = 0;
  len += E == 'E' || E == 'e' ? 2 : 1;
  int limit = 0, was_point = 0;
  int i = 0;
  for (; buffer[i] != '\0' && i < len; i++) {
    if (limit > len) break;
    add_char(s, buffer[i], count);
    if (was_point) limit++;
    if (buffer[i] == '.') was_point = 1;
  }
  if (buffer[i - 1] == '.' && (i >= len)) {
    *s -= 1;
    *count -= 1;
  }

  if (i < len && !G_mode && float_ptr == 0) {
    add_char(s, '0', count);
    i++;
    if (i + 1 < len) {
      add_char(s, '.', count);
      i++;
    }
  }

  for (; i < len && !G_mode; i++) {
    add_char(s, '0', count);
  }
  int was_a_discharge_upgrade = 0;
  if (buffer[0] == '1' && getFirstDigit((int)float_ptr) == 9) {
    was_a_discharge_upgrade = 1;
    tmp_count_zero++;
  }
  char *e_buffer =
      buffer[0] == '\0'
          ? E == 'E' ? "E+00" : "e+00"
          : take_e_part(E, (int)float_ptr, was_a_discharge_upgrade, count_zero);

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
  int len_was_down = 0;

  if ((flags->need_pluse || flags->space || flags->below_zero) &&
      ((flags->point > 0 && len - 1 < flags->point) ||
       (flags->below_zero && flags->offset > 0 && len - 1 < flags->offset &&
        flags->zero))) {
    len_was_down = 1;
    len--;
  }
  if (flags->zero && len < flags->offset) {
    if (len_was_down) flags->offset--;
    add_prev_chars(flags->offset, s, count, len, '0');
    len = flags->offset;
  }
  if (len < flags->point && flags->point > 0) {
    add_prev_chars(flags->point, s, count, len, '0');
    len = flags->point;
    if (flags->need_pluse || flags->space) len++;
    **s = '\0';
    if (len_was_down) len++;
  }
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
  } else if ((flags->need_pluse || flags->space || flags->offset > 0) &&
             len < flags->offset) {
    add_prev_chars(flags->offset, s, count, len, ' ');
  }
}

void add_float(int end_ints, char *buffer, char **s, int *count) {
  int limit = -1;
  int i = 0;
  for (; buffer[i] != '\0' && limit <= end_ints; i++) {
    if (limit != -1 || buffer[i] == '.') limit++;
    add_char(s, buffer[i], count);
    if (limit >= end_ints) break;
  }
  if (limit + 1 <= end_ints - 1 && limit == -1) {
    limit++;
    add_char(s, '.', count);
  }
  for (; limit <= end_ints - 1; limit++) add_char(s, '0', count);
}

int add_shortest_representation(char *buffer, int len_of_int, int *count_zero,
                                int *len, int *limit, char **s, int *count) {
  char *new_buffer = set_point(buffer, len_of_int);
  int tmp_count_zero = (new_buffer[0] - 0 == 0 && len_of_int == 1)
                           ? *count_zero - 1
                           : *count_zero;
  if (new_buffer[0] - 0 == 0 && len_of_int == 1) *count_zero -= 1;
  clear_ending_zeros(new_buffer, *len, len_of_int, tmp_count_zero);
  int time_to_count = 0, wasnt_point = 1;
  for (int i = 0;
       new_buffer[i] != '\0' && *limit < *len &&
       ((new_buffer[i] > 47 && new_buffer[i] < 58) || new_buffer[i] == '.');
       i++) {
    if (new_buffer[i] != '0' && new_buffer[i] != '.') time_to_count = 1;
    if (time_to_count && new_buffer[i] != '.') *limit += 1;
    add_char(s, new_buffer[i], count);
    if (new_buffer[i] == '.') wasnt_point = 0;
  }
  if (*len == 0) *len = 1;
  return wasnt_point;
}

char *repeat_arr_for_shortest_representation(char *buffer) {
  static char buffer_2[17];
  int i = 0;
  for (; buffer[i] != '\0'; i++) buffer_2[i] = buffer[i];
  buffer_2[i] = '\0';
  return buffer_2;
}

int len_w_z_for_s_rep(int len, int limit, char *buffer) {
  int len_without_zero = limit, tmp_time_to_count = 0;
  for (int i = 0; buffer[i] != '\0' && len_without_zero < len; i++) {
    if (buffer[i] != '0') tmp_time_to_count = 1;
    if (tmp_time_to_count) len_without_zero++;
  }
  return len_without_zero;
}

int sientific_if(int len_of_int, int len, int point) {
  return (len_of_int >= len &&
          ((point > 0 && len < len_of_int) || len < len_of_int) &&
          len_of_int != 1);
}

char *make_ready_arr(int *was_greade, char *buffer_2, int tmp_count_zero,
                     int len, int len_of_int) {
  int count_zero_in_scintific = 0;
  *was_greade += rounding_all_fractional(buffer_2, tmp_count_zero,
                                         &count_zero_in_scintific, len - 1);
  char *new_buffer = set_scientific_point(buffer_2);
  clear_ending_zeros(new_buffer, len, len_of_int, count_zero_in_scintific);
  return new_buffer;
}