#include "vigenere.h"
#include <cctype>

VigenereCipher::VigenereCipher(const std::string &key) : key(key) {}

std::string VigenereCipher::prepareKey(const std::string &text) {
    std::string preparedKey;
    for (size_t i = 0, j = 0; i < text.size(); ++i) {
        if (isalpha(text[i]) || isdigit(text[i])) {
            preparedKey += key[j % key.size()];
            j++;
        } else {
            preparedKey += text[i]; // Специальные символы остаются без изменений
        }
    }
    return preparedKey;
}

char VigenereCipher::shiftChar(unsigned char c, unsigned char keyChar, bool encrypt) {
    if (isalpha(c)) {
        char base = isupper(c) ? 'A' : 'a'; // Латиница (верхний и нижний регистры)
        int shift = (keyChar - base) % 26; // Для латиницы
        return encrypt ? (c - base + shift) % 26 + base : (c - base - shift + 26) % 26 + base;
    } else if (isdigit(c)) {
        int shift = (keyChar - '0') % 10; // Смещение для цифр
        return encrypt ? (c - '0' + shift) % 10 + '0' : (c - '0' - shift + 10) % 10 + '0';
    } else {
        return c; // Специальные символы остаются без изменений
    }
}

std::string VigenereCipher::encrypt(const std::string &text) {
    std::string result;
    std::string preparedKey = prepareKey(text);
    for (size_t i = 0; i < text.size(); ++i) {
        if (isalpha(text[i]) || isdigit(text[i])) {
            result += shiftChar(text[i], preparedKey[i], true);
        } else {
            result += text[i];
        }
    }
    return result;
}

std::string VigenereCipher::decrypt(const std::string &text) {
    std::string result;
    std::string preparedKey = prepareKey(text);
    for (size_t i = 0; i < text.size(); ++i) {
        if (isalpha(text[i]) || isdigit(text[i])) {
            result += shiftChar(text[i], preparedKey[i], false);
        } else {
            result += text[i];
        }
    }
    return result;
}