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
        makeRecord(101, "Amit Patil", 85.5F),
        makeRecord(102, "Neha Joshi", 91.0F),
        makeRecord(103, "Ravi Kulkarni", 78.0F)
    };

    {
        std::ofstream outputFile("students.dat", std::ios::binary | std::ios::trunc);
        if (!outputFile) {
            std::cerr << "Error: Could not create students.dat\n";
            return 1;
        }
        for (const StudentRecord& record : records) {
            outputFile.write(reinterpret_cast<const char*>(&record), sizeof(record));
        }
        std::cout << records.size() << " records written to students.dat\n";
    }

    std::ifstream inputFile("students.dat", std::ios::binary);
    if (!inputFile) {
        std::cerr << "Error: Could not open students.dat\n";
        return 1;
    }

    StudentRecord student{};
    int count = 0;
    while (inputFile.read(reinterpret_cast<char*>(&student), sizeof(student))) {
        ++count;
        std::cout << "Record " << count << " -> Roll: " << student.rollNumber
                  << ", Name: " << student.name
                  << ", Marks: " << student.marks << '\n';
    }

    std::cout << "Total records read: " << count << '\n';
    return 0;
}
