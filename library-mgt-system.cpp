#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    bool isBorrowed;
public:
    Book() : id(0), isBorrowed(false) {}
    Book(int _id, const string &_title, const string &_author)
        : id(_id), title(_title), author(_author), isBorrowed(false) {}
    int getId() const { return id; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    bool getIsBorrowed() const { return isBorrowed; }
    void setId(int newId) { id = newId; }
    void setTitle(const string &newTitle) { title = newTitle; }
    void setAuthor(const string &newAuthor) { author = newAuthor; }
    void setIsBorrowed(bool borrowed) { isBorrowed = borrowed; }
};

class Library {
private:
    vector<Book> books;
public:
    void loadFromFile(const string &filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cout << "Could not open " << filename << " for reading.\n";
            return;
        }
        books.clear();
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            istringstream iss(line);
            int id, borrowedInt;
            if (!(iss >> id >> borrowedInt))
                break;
            string title, author;
            if (!getline(inFile, title))
                break;
            if (!getline(inFile, author))
                break;
            Book b(id, title, author);
            b.setIsBorrowed(borrowedInt == 1);
            books.push_back(b);
        }
        inFile.close();
        cout << "Loaded " << books.size() << " books from " << filename << ".\n";
    }

    void saveToFile(const string &filename) {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Could not open " << filename << " for writing.\n";
            return;
        }
        for (const auto &b : books) {
            outFile << b.getId() << " " << (b.getIsBorrowed() ? 1 : 0) << "\n";
            outFile << b.getTitle() << "\n";
            outFile << b.getAuthor() << "\n";
        }
        outFile.close();
        cout << "Saved " << books.size() << " books to " << filename << ".\n";
    }

    void addBook() {
        int id;
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Book Title: ";
        string title;
        getline(cin, title);
        cout << "Enter Book Author: ";
        string author;
        getline(cin, author);
        for (const auto &b : books) {
            if (b.getId() == id) {
                cout << "A book with that ID already exists.\n";
                return;
            }
        }
        books.push_back(Book(id, title, author));
        cout << "Book added.\n";
    }

    void removeBook() {
        int id;
        cout << "Enter Book ID to remove: ";
        cin >> id;
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->getId() == id) {
                books.erase(it);
                cout << "Book removed.\n";
                return;
            }
        }
        cout << "No book found with ID " << id << ".\n";
    }

    void viewAllBooks() {
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;
        }
        cout << "\n----- Book List -----\n";
        for (const auto &b : books) {
            cout << "ID: " << b.getId()
                 << ", Title: " << b.getTitle()
                 << ", Author: " << b.getAuthor()
                 << ", Borrowed: " << (b.getIsBorrowed() ? "Yes" : "No") << "\n";
        }
        cout << "---------------------\n";
    }

    void borrowBook() {
        int id;
        cout << "Enter Book ID to borrow: ";
        cin >> id;
        for (auto &b : books) {
            if (b.getId() == id) {
                if (b.getIsBorrowed()) {
                    cout << "That book is already borrowed.\n";
                    return;
                }
                b.setIsBorrowed(true);
                cout << "You borrowed \"" << b.getTitle() << "\".\n";
                return;
            }
        }
        cout << "No book found with ID " << id << ".\n";
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;
        for (auto &b : books) {
            if (b.getId() == id) {
                if (!b.getIsBorrowed()) {
                    cout << "That book wasn't borrowed.\n";
                    return;
                }
                b.setIsBorrowed(false);
                cout << "You returned \"" << b.getTitle() << "\".\n";
                return;
            }
        }
        cout << "No book found with ID " << id << ".\n";
    }
};

int main() {
    Library lib;
    const string filename = "books.txt";
    lib.loadFromFile(filename);
    while (true) {
        cout << "\n=== LIBRARY MENU ===\n"
             << "1. Add Book\n"
             << "2. Remove Book\n"
             << "3. View All Books\n"
             << "4. Borrow Book\n"
             << "5. Return Book\n"
             << "6. Save & Exit\n"
             << "Choice: ";
        int choice;
        cin >> choice;
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.removeBook(); break;
            case 3: lib.viewAllBooks(); break;
            case 4: lib.borrowBook(); break;
            case 5: lib.returnBook(); break;
            case 6:
                lib.saveToFile(filename);
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}
