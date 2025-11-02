#include "core/librarian.h"
#include "core/book.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
using json = nlohmann::json;

// Initialize static member
Librarian *Librarian::m_instance = nullptr;
std::string Librarian::save_location = "";

Librarian::Librarian() { loadUsers(); }

Librarian::~Librarian() { saveUsers(); }

void CreateDirectories(std::string filename) {
  std::string::iterator it = filename.end() - 1;
  for (; it > filename.begin(); --it) {
    if (*it == '/') {
      --it;
      break;
    }
  }
  std::filesystem::create_directories(
      std::filesystem::current_path() /
      std::filesystem::path(std::string(filename.begin(), it)));
}

void Librarian::saveUsers() {
  CreateDirectories(save_location);
  std::ofstream file(save_location);
  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file for writing: " +
                             save_location);
  }
  json data = {
      {  "version",                                         1},
      {"librarian", {{"books", m_books.log()}, {"users", {}}}}
  };
  for (const auto &i : m_users) {
    json user = {
        {"username", i->getUsername()},
        {   "books", i->m_books.log()}
    };
    data["librarian"]["users"].push_back(user);
  }

  file << data.dump(4);
  file.close();
}

void Librarian::loadUsers() {
  std::ifstream file(save_location);
  if (!file.is_open()) {
    saveUsers();
    return;
  }
  json data = json::parse(file);
  if (data.contains("librarian"))
    m_books.read(data["librarian"]["books"].dump());
  for (size_t i = 0; i < data["librarian"]["users"].size(); ++i) {
    addUser(data["librarian"]["users"][i].value("username", ""));
    findUser(data["librarian"]["users"][i].value("username", ""))
        ->m_books.read(data["librarian"]["users"][i]["books"].dump());
  }
}

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
