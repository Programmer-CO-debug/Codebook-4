#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
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
        "C++ File Handling 2026\n"
        "Students learn file streams.\n";
    createIfMissing("stats_sample.txt", sample);

    std::string fileName;
    std::cout << "Enter file name (press Enter for stats_sample.txt): ";
    std::getline(std::cin, fileName);
    if (fileName.empty()) {
        fileName = "stats_sample.txt";
    }

    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error: Could not open " << fileName << '\n';
        return 1;
    }

    std::size_t lines = 0;
    std::size_t words = 0;
    std::size_t characters = 0;
    std::size_t vowels = 0;
    std::size_t consonants = 0;
    std::size_t digits = 0;
    std::size_t spaces = 0;
    std::size_t punctuation = 0;
    std::size_t frequency[256] = {};
    bool insideWord = false;

    char ch;
    while (inputFile.get(ch)) {
        ++characters;
        unsigned char uc = static_cast<unsigned char>(ch);

        if (ch == '\n') {
            ++lines;
        }

        if (std::isspace(uc)) {
            if (ch == ' ') {
                ++spaces;
            }
            insideWord = false;
        } else {
            if (!insideWord) {
                ++words;
                insideWord = true;
            }
            // Case-insensitive frequency count of non-space characters
            ++frequency[std::tolower(uc)];
        }

        if (std::isalpha(uc)) {
            if (isVowel(ch)) {
                ++vowels;
            } else {
                ++consonants;
            }
        } else if (std::isdigit(uc)) {
            ++digits;
        } else if (std::ispunct(uc)) {
            ++punctuation;
        }
    }

    if (characters > 0) {
        inputFile.clear();
        inputFile.seekg(-1, std::ios::end);
        char lastCharacter;
        inputFile.get(lastCharacter);
        if (lastCharacter != '\n') {
            ++lines;
        }
    }

    // Find the most frequent character
    std::size_t bestCount = 0;
    int bestChar = -1;
    for (int i = 0; i < 256; ++i) {
        if (frequency[i] > bestCount) {
            bestCount = frequency[i];
            bestChar = i;
        }
    }

    std::ostringstream report;
    report << "File Statistics for " << fileName << "\n";
    report << "Lines: " << lines << '\n';
    report << "Words: " << words << '\n';
    report << "Characters: " << characters << '\n';
    report << "Vowels: " << vowels << '\n';
    report << "Consonants: " << consonants << '\n';
    report << "Digits: " << digits << '\n';
    report << "Spaces: " << spaces << '\n';
    report << "Punctuation: " << punctuation << '\n';
    if (bestChar >= 0 && bestCount > 0) {
        report << "Most frequent character: '" << static_cast<char>(bestChar)
               << "' (" << bestCount << " times, case-insensitive)\n";
    } else {
        report << "Most frequent character: none\n";
    }

    std::cout << '\n' << report.str();

    std::ofstream reportFile("report.txt");
    if (reportFile) {
        reportFile << report.str();
        std::cout << "\nReport saved to report.txt\n";
    } else {
        std::cerr << "Error: Could not create report.txt\n";
        return 1;
    }

    return 0;
}
