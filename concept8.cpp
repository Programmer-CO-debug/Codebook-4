#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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
    if (!inputFile) {
        std::cerr << "Error: Could not open students.txt\n";
        return 1;
    }

    std::vector<Student> students;
    std::string line;
    while (std::getline(inputFile, line)) {
        Student student{};
        if (parseStudent(line, student)) {
            students.push_back(student);
        }
    }
    inputFile.close();

    // Formatted table of all records
    std::cout << "\nSTUDENT RECORDS\n";
    std::cout << std::left
              << std::setw(8) << "Roll"
              << std::setw(18) << "Name"
              << std::setw(20) << "Course"
              << std::setw(14) << "Mobile"
              << std::setw(8) << "Marks" << '\n';
    std::cout << std::string(68, '-') << '\n';

    std::cout << std::fixed << std::setprecision(1);
    for (const Student& student : students) {
        std::cout << std::setw(8) << student.rollNumber
                  << std::setw(18) << student.name
                  << std::setw(20) << student.course
                  << std::setw(14) << student.mobile
                  << std::setw(8) << student.marks << '\n';
    }

    // Search by roll number
    int targetRollNumber;
    std::cout << "\nEnter roll number to search: ";
    if (!(std::cin >> targetRollNumber)) {
        std::cerr << "Invalid roll number.\n";
        return 1;
    }

    bool found = false;
    for (const Student& student : students) {
        if (student.rollNumber == targetRollNumber) {
            std::cout << "Record Found\n";
            std::cout << "Roll Number: " << student.rollNumber << '\n';
            std::cout << "Name: " << student.name << '\n';
            std::cout << "Course: " << student.course << '\n';
            std::cout << "Mobile: " << student.mobile << '\n';
            std::cout << "Marks: " << student.marks << '\n';
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Student record not found.\n";
    }
    return 0;
}
