#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

struct StudentRecord {
    int rollNumber;
    char name[30];
    float marks;
};

StudentRecord makeRecord(int rollNumber, const char* name, float marks) {
    StudentRecord student{};
    student.rollNumber = rollNumber;
    std::strncpy(student.name, name, sizeof(student.name) - 1);
    student.marks = marks;
    return student;
}

int main() {
    const std::vector<StudentRecord> records = {
        makeRecord(101, "Amit", 85.5F),
        makeRecord(102, "Neha", 91.0F),
        makeRecord(103, "Ravi", 78.0F)
    };

    {
        std::ofstream outputFile("records.dat", std::ios::binary | std::ios::trunc);
        if (!outputFile) {
            std::cerr << "Error: Could not create records.dat\n";
            return 1;
        }
        for (const StudentRecord& record : records) {
            outputFile.write(reinterpret_cast<const char*>(&record), sizeof(record));
        }
    }

    std::ifstream inputFile("records.dat", std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open records.dat\n";
        return 1;
    }

    int targetRollNumber;
    std::cout << "Enter roll number to search: ";
    if (!(std::cin >> targetRollNumber)) {
        std::cerr << "Invalid roll number.\n";
        return 1;
    }

    // Records have fixed size, so each record can be located by its position.
    // We scan record by record and use tellg() to find the record number.
    StudentRecord selectedStudent{};
    bool found = false;
    while (inputFile.read(reinterpret_cast<char*>(&selectedStudent), sizeof(selectedStudent))) {
        if (selectedStudent.rollNumber == targetRollNumber) {
            std::streamoff endPosition = inputFile.tellg();
            std::streamoff recordNumber =
                endPosition / static_cast<std::streamoff>(sizeof(StudentRecord));

            std::cout << "Record found at record number " << recordNumber << '\n';
            std::cout << "Roll Number: " << selectedStudent.rollNumber << '\n';
            std::cout << "Name: " << selectedStudent.name << '\n';
            std::cout << "Marks: " << selectedStudent.marks << '\n';
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "No record with roll number " << targetRollNumber << " found.\n";
    }
    return 0;
}
