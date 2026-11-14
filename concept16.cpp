#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

const char* kBookFile = "library_books.txt";

class Book {
private:
    int bookId;
    std::string title;
    std::string author;
    bool issued;

public:
    Book(int id, std::string bookTitle, std::string bookAuthor, bool issueStatus = false)
        : bookId(id),
          title(std::move(bookTitle)),
          author(std::move(bookAuthor)),
          issued(issueStatus) {}

    int getBookId() const {
        return bookId;
    }

    bool isIssued() const {
        return issued;
    }

    void setIssued(bool status) {
        issued = status;
    }

    std::string toFileRecord() const {
        return std::to_string(bookId) + "|" + title + "|" + author + "|" +
               (issued ? "1" : "0");
    }

    void display() const {
        std::cout << "Book ID: " << bookId << '\n';
        std::cout << "Title: " << title << '\n';
        std::cout << "Author: " << author << '\n';
        std::cout << "Status: " << (issued ? "Issued" : "Available") << '\n';
    }

    static bool fromFileRecord(const std::string& line, Book& book) {
        std::stringstream record(line);
        std::string idText;
        std::string title;
        std::string author;
        std::string issuedText;
        try {
            if (!std::getline(record, idText, '|') ||
                !std::getline(record, title, '|') ||
                !std::getline(record, author, '|') ||
                !std::getline(record, issuedText)) {
                return false;
            }
            book = Book(std::stoi(idText), title, author, issuedText == "1");
        } catch (const std::exception&) {
            return false;
        }
        return true;
    }
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

std::vector<Book> loadBooks() {
    std::vector<Book> books;
    std::ifstream inputFile(kBookFile);
    std::string line;
    while (std::getline(inputFile, line)) {
        Book book(0, "", "");
        if (Book::fromFileRecord(line, book)) {
            books.push_back(book);
        }
    }
    return books;
}

bool saveBooks(const std::vector<Book>& books) {
    std::ofstream outputFile(kBookFile, std::ios::trunc);
    if (!outputFile) {
        return false;
    }
    for (const Book& book : books) {
        outputFile << book.toFileRecord() << '\n';
    }
    return true;
}

int findBookIndex(const std::vector<Book>& books, int bookId) {
    for (std::size_t i = 0; i < books.size(); ++i) {
        if (books[i].getBookId() == bookId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int readBookId(const std::string& prompt) {
    int id;
    std::cout << prompt;
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return id;
}

void addBook(std::vector<Book>& books) {
    int id = readBookId("Enter book ID: ");

    // Extension 3: prevent duplicate book IDs.
    if (findBookIndex(books, id) != -1) {
        std::cout << "Error: A book with ID " << id << " already exists.\n";
        return;
    }

    std::string title;
    std::string author;
    std::cout << "Enter title: ";
    std::getline(std::cin, title);
    std::cout << "Enter author: ";
    std::getline(std::cin, author);

    books.emplace_back(id, title, author);

    if (saveBooks(books)) {
        std::cout << "Book added successfully.\n";
    } else {
        std::cerr << "Error: Could not save library_books.txt\n";
    }
}

void displayBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "No books in the library.\n";
        return;
    }
    for (const Book& book : books) {
        book.display();
        std::cout << "-------------------------\n";
    }
}

// Extension 1: search a book by ID.
void searchBook(const std::vector<Book>& books) {
    int id = readBookId("Enter book ID to search: ");
    int index = findBookIndex(books, id);
    if (index == -1) {
        std::cout << "Book not found.\n";
        return;
    }
    books[index].display();
}

// Extension 2: issue or return a book by updating its status.
void changeStatus(std::vector<Book>& books, bool issue) {
    int id = readBookId(issue ? "Enter book ID to issue: "
                              : "Enter book ID to return: ");
    int index = findBookIndex(books, id);
    if (index == -1) {
        std::cout << "Book not found.\n";
        return;
    }
    if (issue && books[index].isIssued()) {
        std::cout << "This book is already issued.\n";
        return;
    }
    if (!issue && !books[index].isIssued()) {
        std::cout << "This book was not issued.\n";
        return;
    }

    books[index].setIssued(issue);

    if (saveBooks(books)) {
        std::cout << (issue ? "Book issued successfully.\n"
                            : "Book returned successfully.\n");
    } else {
        std::cerr << "Error: Could not save library_books.txt\n";
    }
}

int main() {
    const std::string sample =
        "101|Let Us C|Yashavant Kanetkar|0\n"
        "102|Data Structures Using C|Reema Thareja|0\n"
        "103|Clean Code|Robert C. Martin|1\n";

    if (!createIfMissing(kBookFile, sample)) {
        std::cerr << "Error: Could not create " << kBookFile << '\n';
        return 1;
    }

    std::vector<Book> books = loadBooks();
    int choice = -1;

    do {
        std::cout << "\nLibrary Record System\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Display All Books\n";
        std::cout << "3. Search Book by ID\n";
        std::cout << "4. Issue Book\n";
        std::cout << "5. Return Book\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice)) {
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addBook(books); break;
            case 2: displayBooks(books); break;
            case 3: searchBook(books); break;
            case 4: changeStatus(books, true); break;
            case 5: changeStatus(books, false); break;
            case 0: std::cout << "Exiting program.\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
