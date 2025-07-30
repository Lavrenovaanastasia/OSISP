#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "morse.h"

typedef struct {
    char ch;
    const char *code;
} MorseEntry;

static MorseEntry morse_table[] = {
    {'A', ".-"},   {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},  {'E', "."},
    {'F', "..-."}, {'G', "--."},  {'H', "...."}, {'I', ".."},   {'J', ".---"},
    {'K', "-.-"},  {'L', ".-.."}, {'M', "--"},   {'N', "-."},   {'O', "---"},
    {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
    {'U', "..-"},  {'V', "...-"}, {'W', ".--"},  {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."}, {'0', "-----"},{'1', ".----"},{'2', "..---"},{'3', "...--"},
    {'4', "....-"},{'5', "....."},{'6', "-...."},{'7', "--..."},{'8', "---.."},
    {'9', "----."},{' ', "/"},   {'.', ".-.-.-"},{',', "--..--"}, {'?', "..--.."},
    {'-', "-....-"},{':', "---..."},{';', "-.-.-."},{'=', "-...-"},{'_', "..--.-"},
    {'"', ".-..-."},{'$', "...-..-"},{'@', ".--.-."},{'!', "-.-.--"},{'\n', "\n"}
};

static const int morse_table_size = sizeof(morse_table) / sizeof(MorseEntry);

const char* char_to_morse(char ch) {
    ch = toupper(ch);
    for (int i = 0; i < morse_table_size; ++i)
        if (morse_table[i].ch == ch) return morse_table[i].code;
    return NULL;
}

char morse_to_char(const char* code) {
    for (int i = 0; i < morse_table_size; ++i)
        if (strcmp(morse_table[i].code, code) == 0) return morse_table[i].ch;
    return '?'; // Неизвестный код
}

void encrypt_text(const char* input, FILE* output) {
  for (size_t i = 0; i < strlen(input); ++i) {
    char ch = input[i];

    if (ch == '\n') {
      fputc('\n', output);
      continue;
    }

    const char* code = char_to_morse(ch);
    if (code) {
      fprintf(output, "%s ", code);
    } else {
      fprintf(stderr, "[!] Предупреждение: символ '%c' (код %d) на позиции %zu не имеет представления в Морзе и был отброшен.\n", ch, ch, i);
    }
  }
}

void decrypt_text(const char* input, FILE* output) {
  char buffer[16];
  int idx = 0;

  for (size_t i = 0; i <= strlen(input); ++i) {
    char c = input[i];

    if (c == '\n') {
      fputc('\n', output);
    } else if (c != ' ' && c != '\0') {
      buffer[idx++] = c;
      if (idx >= 15) {
        buffer[15] = '\0';
        fprintf(stderr, "[!] Предупреждение: слишком длинная комбинация '%s' на позиции %zu. Ограничено 15 символами.\n", buffer, i);
        fprintf(output, "[???]");
        idx = 0;
      }
    } else {
      buffer[idx] = '\0';
      if (idx > 0) {
        char ch = morse_to_char(buffer);
        if (ch == '?') {
          fprintf(stderr, "[!] Предупреждение: нераспознанная комбинация '%s' на позиции %zu. Выводим как [???].\n", buffer, i);
          fprintf(output, "[???]");
        } else {
          fputc(ch, output);
        }
      }
      idx = 0;
    }
  }
}
