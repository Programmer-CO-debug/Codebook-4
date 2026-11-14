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

    std::ifstream sourceFile("message.txt");
    std::ofstream destinationFile("cpp_lines.txt");

    if (!sourceFile) {
        std::cerr << "Error: Could not open source file.\n";
        return 1;
    }
    if (!destinationFile) {
        std::cerr << "Error: Could not create cpp_lines.txt.\n";
        return 1;
    }

    std::string line;
    std::size_t copiedCount = 0;
    while (std::getline(sourceFile, line)) {
        if (line.find("C++") != std::string::npos) {
            destinationFile << line << '\n';
            ++copiedCount;
        }
    }

    std::cout << copiedCount << " line(s) containing C++ copied to cpp_lines.txt\n";
    return 0;
}
