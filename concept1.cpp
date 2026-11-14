#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ofstream outputFile("notes.txt");
    if (!outputFile) {
        std::cerr << "Error: Could not create notes.txt\n";
        return 1;
    }

    std::string line;
    for (int i = 1; i <= 3; ++i) {
        std::cout << "Enter line " << i << ": ";
        std::getline(std::cin, line);
        outputFile << line << '\n';
    }

    outputFile.close();
    std::cout << "Three lines saved to notes.txt\n";
    return 0;
}
