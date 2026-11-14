#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

bool isValidMobile(const std::string& mobile) {
    if (mobile.size() != 10) {
        return false;
    }
    for (char ch : mobile) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}

int main() {
    // Append mode creates students.txt automatically if it does not exist.
    std::ofstream outputFile("students.txt", std::ios::app);
    if (!outputFile) {
        std::cerr << "Error: Could not open students.txt\n";
        return 1;
    }

    int rollNumber;
    std::string name;
    std::string course;
    std::string mobile;
    double marks;

    std::cout << "Enter roll number: ";
    std::cin >> rollNumber;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter name: ";
    std::getline(std::cin, name);

    std::cout << "Enter course: ";
    std::getline(std::cin, course);

    while (true) {
        std::cout << "Enter 10-digit mobile number: ";
        if (!std::getline(std::cin, mobile)) {
            return 1;
        }
        if (isValidMobile(mobile)) {
            break;
        }
        std::cout << "Invalid mobile number. Enter exactly 10 digits.\n";
    }

    std::cout << "Enter marks: ";
    std::cin >> marks;

    outputFile << rollNumber << '|' << name << '|' << course << '|'
               << mobile << '|' << marks << '\n';

    std::cout << "Student record saved successfully.\n";
    return 0;
}

