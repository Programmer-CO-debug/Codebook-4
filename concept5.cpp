#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

bool createIfMissing(const std::string& fileName, const std::string& content) {
    std::ifstream check(fileName);
    if (check) {
        return true;
    }
    std::ofstream out(fileName);
    if (!out) {
        return false;
    }
    out << content;
    return true;
}

bool isVowel(char ch) {
    ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}

int main() {
    const std::string sample =
        "Welcome to C++ File Handling\n"
        "This is the first line written to a file.\n"
        "Files store data permanently.\n"
        "C++ supports object-oriented programming.\n";

    if (!createIfMissing("message.txt", sample)) {
        std::cerr << "Error: Could not create message.txt\n";
        return 1;
    }

    std::ifstream inputFile("message.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open message.txt\n";
        return 1;
    }

    std::size_t lineCount = 0;
    std::size_t wordCount = 0;
    std::size_t characterCount = 0;
    std::size_t vowelCount = 0;
    std::size_t consonantCount = 0;
    std::size_t digitCount = 0;
    std::size_t spaceCount = 0;
    std::size_t punctuationCount = 0;
    bool insideWord = false;

    char ch;
    while (inputFile.get(ch)) {
        ++characterCount;
        unsigned char uc = static_cast<unsigned char>(ch);

        if (ch == '\n') {
            ++lineCount;
        }

        if (std::isspace(uc)) {
            if (ch == ' ') {
                ++spaceCount;
            }
            insideWord = false;
        } else if (!insideWord) {
            ++wordCount;
            insideWord = true;
        }

        if (std::isalpha(uc)) {
            if (isVowel(ch)) {
                ++vowelCount;
            } else {
                ++consonantCount;
            }
        } else if (std::isdigit(uc)) {
            ++digitCount;
        } else if (std::ispunct(uc)) {
            ++punctuationCount;
        }
    }

    // Count the last line if the file does not end with a newline.
    if (characterCount > 0) {
        inputFile.clear();
        inputFile.seekg(-1, std::ios::end);
        char lastCharacter;
        inputFile.get(lastCharacter);
        if (lastCharacter != '\n') {
            ++lineCount;
        }
    }

    std::cout << "Lines:        " << lineCount << '\n';
    std::cout << "Words:        " << wordCount << '\n';
    std::cout << "Characters:   " << characterCount << '\n';
    std::cout << "Vowels:       " << vowelCount << '\n';
    std::cout << "Consonants:   " << consonantCount << '\n';
    std::cout << "Digits:       " << digitCount << '\n';
    std::cout << "Spaces:       " << spaceCount << '\n';
    std::cout << "Punctuation:  " << punctuationCount << '\n';
    return 0;
}
