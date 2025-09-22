// Class that contains all user information and methods to interact with the
// user interface

#ifndef USER_H
#define USER_H

#include "book.h"

class User {
private:
  std::string m_username;
  std::string m_thumbnail;

public:
  BookManager m_books;

  User(std::string username = "", std::string thumbnail = "")
      : m_username(username), m_thumbnail(thumbnail) {}

  ~User() {}

  // getters
  std::string getUsername() const { return m_username; }
  std::string getThumbnail() const { return m_thumbnail; }

  // setters
  void setUsername(std::string username) { m_username = username; }
  void setThumbnail(std::string thumbnail) { m_thumbnail = thumbnail; }
};

#endif