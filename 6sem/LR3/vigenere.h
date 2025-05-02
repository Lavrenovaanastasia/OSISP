#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

class VigenereCipher {
public:
    VigenereCipher(const std::string &key);
    std::string encrypt(const std::string &text);
    std::string decrypt(const std::string &text);

private:
    std::string key;
    std::string prepareKey(const std::string &text);
    char shiftChar(unsigned char c, unsigned char keyChar, bool encrypt);
};

#endif // VIGENERE_H