#include <iostream>
#include <string>
#include <vector>
#include "../src/functions.h"

// Структура для тест-кейсов
struct TestCase {
    std::string input;
    std::string expected;
};

// Простая система проверки
void run_tests(const std::vector<TestCase>& tests) {
    int passed = 0;
    for (size_t i = 0; i < tests.size(); ++i) {
        std::string result = encode(tests[i].input);
        std::cout << "Test " << i + 1 << ": ";
        if (result == tests[i].expected) {
            std::cout << "PASSED\n";
            ++passed;
        } else {
            std::cout << "FAILED\n";
            std::cout << "  Input:    " << tests[i].input << "\n";
            std::cout << "  Expected: " << tests[i].expected << "\n";
            std::cout << "  Got:      " << result << "\n";
        }
    }
    std::cout << "\nSummary: " << passed << " / " << tests.size() << " tests passed.\n";
}

int main() {
    std::vector<TestCase> tests = {
        {"HELLO", ".... . .-.. .-.. ---"},
        {"123", ".---- ..--- ...--"},
        {"HI THERE", ".... .. / - .... . .-. ."},
        {"A$B", ".- -..."}, // $ — игнорируется
        {"", ""},            // Пустая строка
        {"morse code", "-- --- .-. ... . / -.-. --- -.. ."},
    };

    run_tests(tests);
    return 0;
}