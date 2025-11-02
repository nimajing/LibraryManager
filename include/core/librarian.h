#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "book.h"
#include "user.h"
#include <memory>

class Librarian {
  private:
  static Librarian *m_instance;
  std::vector<std::unique_ptr<User>> m_users;
  static std::string save_location;
  BookManager m_books;

  Librarian();

  // Prevent copying and assignment
  Librarian(const Librarian &) = delete;
  Librarian &operator=(const Librarian &) = delete;

  void saveUsers();
  void loadUsers();

  public:
  static Librarian *Instance(const std::string& new_path = "") {
    if(!new_path.empty()) save_location = new_path;
    if (!m_instance) {
      m_instance = new Librarian();
    }
    return m_instance;
  }
  ~Librarian();

  // User functions
  bool addUser(const std::string &username);
  bool removeUser(const std::string &username);
  User *findUser(const std::string &isbn);
  bool editUser(const std::string &username, const User &new_data);
  const std::vector<std::unique_ptr<User>> &getUsers() const { return m_users; }

  // Book functions
  void addBook(const Book &b);
  bool removeBook(std::string isbn);
  bool editBook(const std::string isbn, const Book &b);
  std::vector<Book> getBooks() const { return m_books.getBooks(); }
};

#endif
