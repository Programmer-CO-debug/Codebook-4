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

    std::string line;
    int lineNumber = 0;
    std::cout << "File Content:\n";
    while (std::getline(inputFile, line)) {
        ++lineNumber;
        std::cout << lineNumber << ": " << line << '\n';
    }

    inputFile.close();
    return 0;
}
