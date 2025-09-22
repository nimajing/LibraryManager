#include "core/librarian.h"
#include "core/book.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
using json = nlohmann::json;

// Initialize static member
Librarian *Librarian::m_instance = nullptr;

Librarian::Librarian() { /*loadUsers();*/ }

void Librarian::createDataDirectory() {
  std::filesystem::path dirPath = "assets/program_data";
  if (!std::filesystem::exists(dirPath)) {
    std::filesystem::create_directories(dirPath);
  }
}

void Librarian::saveUsers() {
  createDataDirectory();
  std::ofstream file(USER_DATA_PATH);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file for writing: " +
                             USER_DATA_PATH);
  }
  json data = {
    {"version", 1},
    {"librarian", {m_books.log()}}};
  data.push_back({"users", {}});
  for (const auto &i : m_users) {
    json user = {{"username", i->getUsername()}, {"books", i->m_books.log()}};
    data["users"].push_back(user);
  }

  file << data.dump(4);
  file.close();
}

void Librarian::loadUsers() {
  createDataDirectory();
  std::ifstream file(USER_DATA_PATH);
  if (!file.is_open()) {
    saveUsers();
    return;
  }
  json data = json::parse(file);
  if(data.contains("librarian"))
    m_books.read(data["librarian"]);
  for (size_t i = 1; i < data.size(); ++i) {
    addUser(data[i]["username"]);
    findUser(data[i]["username"])->m_books.read(data[i]["books"].dump());
  }
}

Librarian::~Librarian() { saveUsers(); }

bool Librarian::addUser(const std::string &username) {
  if (findUser(username) != nullptr) {
    return false;
  }
  m_users.push_back(std::make_unique<User>(username));
  saveUsers();
  return true;
}

User *Librarian::findUser(const std::string &username) {
  for (auto &i : m_users)
    if (i->getUsername() == username)
      return i.get();
  return nullptr;
}

bool Librarian::removeUser(const std::string &username) {
  auto userPtr = findUser(username);
  if (userPtr) {
    auto iter = std::find_if(m_users.begin(), m_users.end(),
                             [userPtr](auto &i) { return userPtr == i.get(); });
    if (iter != m_users.end()) {
      m_users.erase(iter);
      saveUsers();
      return true;
    }
  }
  return false;
}

bool Librarian::editUser(const std::string &username, const User &new_data) {
  auto i = findUser(username);
  if (i)
    *i = new_data;
  else
    return false;
  return true;
}

void Librarian::addBook(const Book &b) { m_books.bring(b); }

bool Librarian::removeBook(const std::string isbn) {
  return m_books.remove(isbn);
}