#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

struct Student {
    int rollNumber;
    std::string name;
    std::string course;
    std::string mobile;
    double marks;
};

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

bool parseStudent(const std::string& line, Student& student) {
    std::stringstream record(line);
    std::string rollText;
    std::string marksText;
    try {
        if (!std::getline(record, rollText, '|') ||
            !std::getline(record, student.name, '|') ||
            !std::getline(record, student.course, '|') ||
            !std::getline(record, student.mobile, '|') ||
            !std::getline(record, marksText)) {
            return false;
        }
        student.rollNumber = std::stoi(rollText);
        student.marks = std::stod(marksText);
    } catch (const std::exception&) {
        return false;
    }
    return true;
}

int main() {
    const std::string sample =
        "101|Amit Patil|B.Tech AI and DS|9876543210|85.5\n"
        "102|Neha Joshi|B.Tech AI and DS|9123456780|91\n"
        "103|Ravi Kulkarni|B.Tech AI and DS|9988776655|78\n";

    if (!createIfMissing("students.txt", sample)) {
        std::cerr << "Error: Could not create students.txt\n";
        return 1;
    }

    std::ifstream inputFile("students.txt");
    std::ofstream temporaryFile("students_temp.txt");
    if (!inputFile || !temporaryFile) {
        std::cerr << "Error: Could not open file(s).\n";
        return 1;
    }

    int targetRollNumber;
    std::cout << "Enter roll number to update: ";
    std::cin >> targetRollNumber;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string newName;
    std::cout << "Enter new name: ";
    std::getline(std::cin, newName);

    double newMarks;
    std::cout << "Enter new marks: ";
    std::cin >> newMarks;

    std::string line;
    bool found = false;
    while (std::getline(inputFile, line)) {
        Student student{};
        if (parseStudent(line, student) && student.rollNumber == targetRollNumber) {
            temporaryFile << student.rollNumber << '|' << newName << '|'
                          << student.course << '|' << student.mobile << '|'
                          << newMarks << '\n';
            found = true;
        } else {
            temporaryFile << line << '\n';
        }
    }

    inputFile.close();
    temporaryFile.close();

    if (!found) {
        std::remove("students_temp.txt");
        std::cout << "Student record not found. No update performed.\n";
        return 0;
    }

    if (std::remove("students.txt") != 0) {
        std::cerr << "Error: Could not remove old students.txt\n";
        return 1;
    }
    if (std::rename("students_temp.txt", "students.txt") != 0) {
        std::cerr << "Error: Could not rename temporary file.\n";
        return 1;
    }

    std::cout << "Student name and marks updated successfully.\n";
    return 0;
}
