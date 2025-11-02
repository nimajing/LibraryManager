#pragma once

#include "json.hpp"
#include <string>
#include <vector>

class Book {
  private:
  std::string m_title;
  std::vector<std::string> m_authors;
  std::string m_isbn;
  int m_copies;
  std::string m_thumbnail;

  public:
  Book(std::string title = "", std::vector<std::string> authors = {},
       std::string isbn = "", int copies = 0, std::string thumbnail = "") {
    m_title = title;
    m_authors = authors;
    m_isbn = isbn;
    m_copies = copies;
    m_thumbnail = thumbnail;
  }

  ~Book() {}

  // Add rule of five
  Book(const Book &other)
      : m_title(other.m_title), m_authors(other.m_authors),
        m_isbn(other.m_isbn), m_copies(other.m_copies),
        m_thumbnail(other.m_thumbnail) {}

  Book &operator=(const Book &other) {
    if (this != &other) {
      m_title = other.m_title;
      m_authors = other.m_authors;
      m_isbn = other.m_isbn;
      m_copies = other.m_copies;
      m_thumbnail = other.m_thumbnail;
    }
    return *this;
  }

  // Move constructor
  Book(Book &&other) noexcept = default;

  // Move assignment
  Book &operator=(Book &&other) noexcept = default;

  // Add these getter methods in the public section:
  std::string getTitle() const { return m_title; }
  std::vector<std::string> getAuthors() const { return m_authors; }
  std::string getIsbn() const { return m_isbn; }
  int getCopies() const { return m_copies; }
  std::string getThumbnail() const { return m_thumbnail; }

  // Setters for the book class
  void setTitle(std::string title);
  void addAuthor(std::string author);
  void removeAuthor(std::string author);
  void editAuthor(std::string oldAuthor, std::string newAuthor);
  void setIsbn(std::string isbn);
  void setCopies(int c);
  void addCopies(int c);
  bool removeCopies(int c);
  void setThumbnail(std::string thumbnail);
};

class BookManager {
  private:
  std::vector<Book> m_books;

  public:
  bool take(std::string isbn, int copies, BookManager &rec);
  void bring(const Book &b);
  bool remove(std::string isbn);
  std::vector<Book> getBooks() const { return m_books; }
  nlohmann::json log();
  bool read(std::string msg);
};