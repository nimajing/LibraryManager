#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "book.h"
#include "user.h"
#include <memory>

class Librarian {
private:
    static Librarian* m_instance;
    std::vector<std::unique_ptr<User>> m_users;
    BookManager m_books;
    const std::string USER_DATA_PATH = "assets/program_data/users.json";
    
    Librarian();
    
    // Prevent copying and assignment
    Librarian(const Librarian&) = delete;
    Librarian& operator=(const Librarian&) = delete;

    void createDataDirectory();
    void saveUsers();
    void loadUsers();

public:
    static Librarian* Instance() {
        if (!m_instance) {
            m_instance = new Librarian();
        }
        return m_instance;
    }
    /**
     * @brief Destructor for Librarian.
     * 
     * Responsible for cleaning up dynamically allocated User objects in m_users
     * to prevent memory leaks.
     */
    ~Librarian();

    // User functions
    bool addUser(const std::string& username);
    bool removeUser(const std::string &username);
    User* findUser(const std::string& isbn);
    bool editUser(const std::string &username, const User &new_data);
    const std::vector<std::unique_ptr<User>>& getUsers() const { return m_users; }

    // Book functions
    void addBook(const Book &b);
    bool removeBook(std::string isbn);
    bool editBook(const std::string isbn, const Book &b);
    std::vector<Book> getBooks() const { return m_books.getBooks(); }
};

#endif