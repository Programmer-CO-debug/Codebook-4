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

// Removes leading and trailing punctuation and converts to lowercase.
// "C++." becomes "c++", "File," becomes "file".
std::string normalize(std::string word) {
    const std::string punctuation = ".,;:!?\"'()[]{}";
    std::size_t start = word.find_first_not_of(punctuation);
    if (start == std::string::npos) {
        return "";
    }
    std::size_t end = word.find_last_not_of(punctuation);
    word = word.substr(start, end - start + 1);

    for (char& c : word) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return word;
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

    std::string searchWord;
    std::cout << "Enter word to search: ";
    std::cin >> searchWord;

    std::string target = normalize(searchWord);
    if (target.empty()) {
        std::cout << "Please enter a valid word.\n";
        return 0;
    }

    std::string word;
    int count = 0;
    while (inputFile >> word) {
        if (normalize(word) == target) {
            ++count;
        }
    }

    std::cout << "The word '" << searchWord << "' occurred " << count << " time(s) "
              << "(case and punctuation ignored).\n";
    return 0;
}
