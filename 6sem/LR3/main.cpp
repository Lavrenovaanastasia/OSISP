#include <iostream>
#include <fstream>
#include "vigenere.h"

void processFile(const std::string &inputFile, const std::string &outputFile, const std::string &key, bool encrypt) {
    std::ifstream in(inputFile);
    std::ofstream out(outputFile);
    if (!in || !out) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    

    VigenereCipher cipher(key);
    std::string line;
    while (std::getline(in, line)) {
        if (encrypt) {
            out << cipher.encrypt(line) << std::endl;
        } else {
            out << cipher.decrypt(line) << std::endl;
        }
    }

    in.close();
    out.close();
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <encrypt/decrypt> <input_file> <output_file> <key>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    std::string key = argv[4];

    bool encrypt = (mode == "encrypt");

    processFile(inputFile, outputFile, key, encrypt);
    return 0;
}