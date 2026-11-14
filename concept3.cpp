#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::string name;
    std::string date;

    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    std::cout << "Enter the date (for example, 14-11-2026): ";
    std::getline(std::cin, date);

    // ios::app creates message.txt if it does not exist, and never overwrites it.
    std::ofstream outputFile("message.txt", std::ios::app);
    if (!outputFile) {
        std::cerr << "Error: Could not open message.txt for appending\n";
        return 1;
    }

    outputFile << "Name: " << name << " | Date: " << date << '\n';
    outputFile.close();

    std::cout << "Name and date appended successfully to message.txt\n";
    return 0;
}
