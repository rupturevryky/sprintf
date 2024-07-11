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
  // printf("1 float_string %s\n", float_string);
  if (len == 0) len = 1;

  int i = strlen(float_string) - 1;
  for (; i >= len && i > 0; i--) {
    if (float_string[i] == '0')
      float_string[i] = '\0';
    else
      break;
  }

  i = int_len + len + prev_zero_count;
  // printf("2 float_string %s; i %d; prev_zero_count %d\n", float_string, i,
  //        prev_zero_count);
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
  // printf("3 float_string %s; i %d\n", float_string, i);
  int point_index = -1, start = -1, zero_count = 0;
  for (int j = 0; float_string[j] != '\0'; j++) {
    if (float_string[j] == '.') point_index = j;
    if (start == -1 && float_string[j] > 48 && float_string[j] < 58) start = j;
    if (start != -1 && zero_count != -1 && point_index > 0 &&
        float_string[j] == '0') {
      zero_count++;
      // printf(
      //     "float_string[%d] %c; zero_count %d; point_index %d; "
      //     "start % d\n ",
      //     i, float_string[i], zero_count, point_index, start);
      if (zero_count + int_len >= len) break;
    } else if (zero_count > 0 ||
               (point_index > 0 && start != -1 && float_string[j] != '.'))
      zero_count = -1;
  }
  if (zero_count > 4) float_string[point_index] = '\0';
  // printf("4 float_string %s; i %d; start %d; ; zero_count %d\n",
  // float_string,
  //        i, start, zero_count);
  for (int j = 0; float_string[j] != '\0'; j++) i = j;
  while ((float_string[i] == '0' || float_string[i] == '.') &&
         i >= point_index && i > 0) {
    // printf(
    //     "float_string[%d] %c; zero_count %d; point_index %d; "
    //     "start % d\n ",
    //     i, float_string[i], zero_count, point_index, start);
    float_string[i] = '\0';
    i--;
  }
  // printf("5 float_string %s; i %d; start %d; ; zero_count %d\n",
  // float_string,
  //        i, start, zero_count);

  i = prev_zero_count + len;
  for (; i >= int_len && i > 0; i--) {
    if (float_string[i] == '0')
      float_string[i] = '\0';
    else
      break;
  }
  // printf("5 float_string %s\n", float_string);
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
  // printf("my float_arr %s\n", float_arr);
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
  // new_str[length + 1] = '\0';
  // printf("my new_str %s\n", new_str);
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
  // printf("scientific_notatio scientific_num %s\n", tmp);
  return tmp;
}

void write_ready_scientific_num(char **s, int *count, char *buffer,
                                int count_zero, double float_ptr, char E,
                                int len, int G_mode) {
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
    // printf("!!!scientific buffer %s; len %c\n", buffer, len);
    *s -= 1;
    *count -= 1;
  }

  // printf("G_mode %d; float_ptr %lf; buffer %s; i %d,len %d; limit %d\n",
  // G_mode,
  //        float_ptr, buffer, i, len, limit);

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
          ? "e+00"
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
  // printf("1 len %d; count: %d; count_start: %d\n", len, *count, count_start);

  if ((flags->need_pluse || flags->space || flags->below_zero) &&
      ((flags->point > 0 && len - 1 < flags->point) ||
       (flags->below_zero && flags->offset > 0 && len - 1 < flags->offset &&
        flags->zero))) {
    len_was_down = 1;
    len--;
  }
  // printf("2 len %d; count: %d; count_start: %d\n", len, *count, count_start);
  if (flags->zero && len < flags->offset) {
    if (len_was_down) flags->offset--;
    add_prev_chars(flags->offset, s, count, len, '0');
    len = flags->offset;
    // printf("offset: %d\n", flags->offset);
    // if (len_was_down) {
    //   len++;
    //   flags->offset++;
    //   len_was_down = 0;
    // }
  }
  if (len < flags->point && flags->point > 0) {
    add_prev_chars(flags->point, s, count, len, '0');
    len = flags->point;
    // printf("len %d\n", len);
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
  }
  // else if (flags->zero && len < flags->offset) {
  //   add_prev_chars(flags->offset, s, count, len, '0');
  // }
  else if ((flags->need_pluse || flags->space || flags->offset > 0) &&
           len < flags->offset) {
    // printf("flags->offset %d; len %d\n", flags->offset, len);
    add_prev_chars(flags->offset, s, count, len, ' ');
  }
}