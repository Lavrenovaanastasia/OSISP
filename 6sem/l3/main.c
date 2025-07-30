#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "morse.h"

void print_usage(const char* progname) {
  printf("Использование:\n");
  printf("  %s -e|-d -i <входной_файл> -o <выходной_файл>\n", progname);
  printf("Опции:\n");
  printf("  -e        шифровать (текст -> Морзе)\n");
  printf("  -d        расшифровать (Морзе -> текст)\n");
  printf("  -i FILE   указать входной файл\n");
  printf("  -o FILE   указать выходной файл\n");
  printf("Если опции не заданы — запускается интерактивный режим.\n");
}

int main(int argc, char* argv[]) {
  if (argc == 6 && (strcmp(argv[1], "-e") == 0  || strcmp(argv[1], "-d") == 0)) {
    const char* infile = NULL;
    const char* outfile = NULL;

    for (int i = 2; i < argc; i++) {
      if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
        infile = argv[++i];
      else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        outfile = argv[++i];
      else {
        print_usage(argv[0]);
        return 1;
      }
    }

    if (!infile || !outfile) {
      fprintf(stderr, "Ошибка: не указаны входной или выходной файл.\n");
      print_usage(argv[0]);
      return 1;
    }

    FILE* in = fopen(infile, "r");
    if (!in) {
      perror("Ошибка открытия входного файла");
      return 1;
    }

    FILE* out = fopen(outfile, "w");
    if (!out) {
      perror("Ошибка открытия выходного файла");
      fclose(in);
      return 1;
    }

    char buffer[4096] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, in);
    fclose(in);

    if (strcmp(argv[1], "-e") == 0) {
      encrypt_text(buffer, out);
    } else {
      decrypt_text(buffer, out);
    }

    fclose(out);
    printf(" Обработка завершена. Результат сохранён в '%s'\n", outfile);
    return 0;
  }

  // === ИНТЕРАКТИВНЫЙ РЕЖИМ ===
  // (Остаётся прежний интерфейс, как описано выше)

  int mode = 0;
  int source = 0;

  printf("Выберите режим:\n1 - Зашифровать (в Морзе)\n2 - Расшифровать (из Морзе)\nВаш выбор: ");
  scanf("%d", &mode);
  getchar();  // поглощение \n

  printf("Откуда взять текст?\n1 - Ввести вручную\n2 - Прочитать из файла (test_input.txt)\nВаш выбор: ");
  scanf("%d", &source);
  getchar();  // поглощение \n

  char input_text[4096] = {0};

  if (source == 1) {
    printf("Введите текст:\n> ");
    fgets(input_text, sizeof(input_text), stdin);
    input_text[strcspn(input_text, "\n")] = 0;
  } else {
    FILE* in = fopen("test_input.txt", "r");
    if (!in) {
      perror("Ошибка открытия файла test_input.txt");
      return 1;
    }
    fread(input_text, 1, sizeof(input_text) - 1, in);
    fclose(in);
  }

  FILE* out = fopen("test_output.txt", "w");
  if (!out) {
    perror("Ошибка записи в файл");
    return 1;
  }

  if (mode == 1)
    encrypt_text(input_text, out);
  else
    decrypt_text(input_text, out);

  fclose(out);
  printf("\n--- Результат сохранён в test_output.txt ---\n");

  FILE* show = fopen("test_output.txt", "r");
  if (show) {
    char buffer[4096] = {0};
    fread(buffer, 1, sizeof(buffer) - 1, show);
    fclose(show);
    printf("Содержимое результата:\n%s\n", buffer);

    printf("Хотите вернуть исходный текст?\n1 - Да\n2 - Нет\nВаш выбор: ");
    int rev = 0;
    scanf("%d", &rev);
    getchar();

    if (rev == 1) {
      printf("\n--- Восстановленный текст ---\n");
      if (mode == 1)
        decrypt_text(buffer, stdout);
      else
        encrypt_text(buffer, stdout);
    }
  }

  return 0;
}