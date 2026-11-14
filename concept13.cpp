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
    // Creates a valid sample file so the user has something to open.
    createIfMissing("message.txt",
        "Welcome to C++ File Handling\n"
        "This is the first line written to a file.\n"
        "Files store data permanently.\n");

    std::ifstream inputFile;
    std::string fileName;

    while (true) {
        std::cout << "Enter file name (type 'exit' to quit): ";
        std::getline(std::cin, fileName);

        if (fileName == "exit") {
            std::cout << "Program ended.\n";
            return 0;
        }

        inputFile.close();
        inputFile.clear();
        inputFile.open(fileName);

        if (inputFile.is_open()) {
            break;
        }

        std::cerr << "Error: Could not open '" << fileName
                  << "'. Check the name and try again.\n";
    }

    std::cout << "\nFile opened successfully. Content:\n";
    std::string line;
    while (std::getline(inputFile, line)) {
        std::cout << line << '\n';
    }

    if (inputFile.eof()) {
        std::cout << "\nEnd of file reached normally.\n";
    } else if (inputFile.bad()) {
        std::cerr << "A serious file I/O error occurred.\n";
    } else if (inputFile.fail()) {
        std::cerr << "A logical file read error occurred.\n";
    }

    inputFile.close();
    return 0;
}
