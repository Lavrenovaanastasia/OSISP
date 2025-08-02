#include "functions.h"
#include <map>
#include <string>
#include <cctype>

std::map<char, std::string> charToMorseCode = {
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
    {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
    {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
    {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
    {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."},

    {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
    {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."}
};

std::string encode(const std::string& text)
{
    std::string result;

    for (size_t i = 0; i < text.size(); ++i)
    {
        char c = std::toupper(text[i]);

        if (c == ' ')
        {
            result += "/ ";
            continue;
        }

        auto it = charToMorseCode.find(c);
        if (it != charToMorseCode.end())
        {
            result += it->second + " ";
        }
    }

    // Удалим последний пробел, если есть
    if (!result.empty() && result.back() == ' ')
        result.pop_back();

    return result;
}