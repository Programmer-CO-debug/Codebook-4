#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

const char* kRecordFile = "student_records.txt";
const char* kTempFile = "student_records_temp.txt";

struct Student {
    int rollNumber;
    std::string name;
    double marks;
};

bool parseStudent(const std::string& line, Student& student) {
    std::stringstream record(line);
    std::string rollText;
    std::string marksText;
    try {
        if (!std::getline(record, rollText, '|') ||
            !std::getline(record, student.name, '|') ||
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

char gradeFor(double marks) {
    if (marks >= 90) return 'A';
    if (marks >= 80) return 'B';
    if (marks >= 70) return 'C';
    if (marks >= 60) return 'D';
    return 'F';
}

std::vector<Student> loadStudents() {
    std::vector<Student> students;
    std::ifstream inputFile(kRecordFile);
    std::string line;
    while (std::getline(inputFile, line)) {
        Student student{};
        if (parseStudent(line, student)) {
            students.push_back(student);
        }
    }
    return students;
}

// Writes to a temporary file, then replaces the original file.
bool saveStudents(const std::vector<Student>& students) {
    {
        std::ofstream temporaryFile(kTempFile);
        if (!temporaryFile) {
            return false;
        }
        for (const Student& student : students) {
            temporaryFile << student.rollNumber << '|' << student.name << '|'
                          << student.marks << '\n';
        }
    }
    std::remove(kRecordFile);
    return std::rename(kTempFile, kRecordFile) == 0;
}

bool rollExists(const std::vector<Student>& students, int rollNumber) {
    for (const Student& student : students) {
        if (student.rollNumber == rollNumber) {
            return true;
        }
    }
    return false;
}

int findIndex(const std::vector<Student>& students, int rollNumber) {
    for (std::size_t i = 0; i < students.size(); ++i) {
        if (students[i].rollNumber == rollNumber) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Keeps asking until the user enters a number from 0 to 100.
double readValidMarks() {
    double marks;
    while (true) {
        std::cout << "Enter marks (0-100): ";
        if (std::cin >> marks && marks >= 0 && marks <= 100) {
            return marks;
        }
        if (std::cin.eof()) {
            std::exit(0);
        }
        std::cout << "Invalid marks. Enter a number between 0 and 100.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void printHeader() {
    std::cout << std::left << std::setw(10) << "Roll No."
              << std::setw(22) << "Name"
              << std::setw(10) << "Marks"
              << std::setw(8) << "Grade" << '\n';
    std::cout << std::string(50, '-') << '\n';
}

void printStudent(const Student& student) {
    std::cout << std::left << std::setw(10) << student.rollNumber
              << std::setw(22) << student.name
              << std::setw(10) << student.marks
              << std::setw(8) << gradeFor(student.marks) << '\n';
}

void addStudent() {
    std::vector<Student> students = loadStudents();

    Student student{};
    std::cout << "Enter roll number: ";
    std::cin >> student.rollNumber;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (rollExists(students, student.rollNumber)) {
        std::cout << "Error: A student with roll number " << student.rollNumber
                  << " already exists.\n";
        return;
    }

    std::cout << "Enter name: ";
    std::getline(std::cin, student.name);

    student.marks = readValidMarks();
    students.push_back(student);

    if (saveStudents(students)) {
        std::cout << "Record added successfully.\n";
    } else {
        std::cerr << "Error: Could not save the record.\n";
    }
}

void displayStudents() {
    std::vector<Student> students = loadStudents();
    if (students.empty()) {
        std::cout << "No student records found.\n";
        return;
    }

    std::cout << '\n';
    printHeader();
    std::cout << std::fixed << std::setprecision(1);
    for (const Student& student : students) {
        printStudent(student);
    }
}

void searchStudent() {
    std::vector<Student> students = loadStudents();

    int targetRoll;
    std::cout << "Enter roll number to search: ";
    std::cin >> targetRoll;

    int index = findIndex(students, targetRoll);
    if (index == -1) {
        std::cout << "Student not found.\n";
        return;
    }

    std::cout << "Record Found\n";
    std::cout << "Roll Number: " << students[index].rollNumber << '\n';
    std::cout << "Name: " << students[index].name << '\n';
    std::cout << "Marks: " << students[index].marks << '\n';
    std::cout << "Grade: " << gradeFor(students[index].marks) << '\n';
}

void updateMarks() {
    std::vector<Student> students = loadStudents();

    int targetRoll;
    std::cout << "Enter roll number to update: ";
    std::cin >> targetRoll;

    int index = findIndex(students, targetRoll);
    if (index == -1) {
        std::cout << "Student not found. No changes made.\n";
        return;
    }

    students[index].marks = readValidMarks();

    if (saveStudents(students)) {
        std::cout << "Marks updated successfully.\n";
    } else {
        std::cerr << "Error: Could not save the record.\n";
    }
}

void deleteStudent() {
    std::vector<Student> students = loadStudents();

    int targetRoll;
    std::cout << "Enter roll number to delete: ";
    std::cin >> targetRoll;

    int index = findIndex(students, targetRoll);
    if (index == -1) {
        std::cout << "Student not found. Nothing deleted.\n";
        return;
    }

    students.erase(students.begin() + index);

    if (saveStudents(students)) {
        std::cout << "Record deleted successfully.\n";
    } else {
        std::cerr << "Error: Could not save the record.\n";
    }
}

int main() {
    int choice = -1;
    do {
        std::cout << "\nStudent Record Manager\n";
        std::cout << "1. Add Student\n";
        std::cout << "2. Display All Students\n";
        std::cout << "3. Search Student\n";
        std::cout << "4. Update Marks\n";
        std::cout << "5. Delete Student\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice)) {
            break;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateMarks(); break;
            case 5: deleteStudent(); break;
            case 0: std::cout << "Exiting program.\n"; break;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
