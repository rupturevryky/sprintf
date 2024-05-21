#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

void mini_scanf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  for (const char *ptr = format; *ptr != '\0'; ++ptr) {
    if (*ptr == '%') {
      ++ptr;
      if (*ptr == 'd') {
        int *int_ptr = va_arg(args, int *);
        scanf("%d", int_ptr);
      } else if (*ptr == 'c') {
        char *char_ptr = va_arg(args, char *);
        // Пропуск пробельных символов перед чтением символа
        while (isspace(*ptr)) ++ptr;
        scanf(" %c", char_ptr);
      }
    }
  }

  va_end(args);
}

int main() {
  int num;
  char ch;

  printf("Введите целое число и символ: ");
  mini_scanf("%d %c", &num, &ch);

  printf("Данные: %d, %c\n", num, ch);

  return 0;
}
