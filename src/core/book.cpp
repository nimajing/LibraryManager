#include "core/book.h"
using json = nlohmann::json;

void Book::setTitle(std::string title) { m_title = title; }

void Book::addAuthor(std::string author) {
  for (const auto &i : m_authors)
    if (i == author)
      return;
  m_authors.push_back(author);
}

void Book::removeAuthor(std::string author) {
  for (auto i = m_authors.begin(); i != m_authors.end(); ++i) {
    if (*i == author) {
      m_authors.erase(i);
      return;
    }
  }
}

void Book::editAuthor(std::string old_author, std::string new_author) {
  for (auto &i : m_authors)
    if (i == old_author) {
      i = new_author;
      return;
    }
}

void Book::setIsbn(std::string isbn) { m_isbn = isbn; }

void Book::setCopies(int c) { m_copies = c; }

void Book::addCopies(int c) { m_copies += c; }

bool Book::removeCopies(int c) {
  if (c > m_copies) {
    return false;
  }
  m_copies -= c;
  return true;
}

void Book::setThumbnail(std::string thumbnail) { m_thumbnail = thumbnail; }

bool BookManager::take(std::string isbn, int copies, BookManager &rec) {
  for (auto i = m_books.begin(); i != m_books.end(); ++i) {
    if (i->getIsbn() == isbn && i->removeCopies(copies)) {
      rec.bring(*i);
      if (!i->getCopies())
        m_books.erase(i);
      return true;
    }
  }
  return false;
}

void BookManager::bring(const Book &b) {
  for (auto &i : m_books) {
    if (i.getIsbn() == b.getIsbn()) {
      i.addCopies(b.getCopies());
      return;
    }
  }
  m_books.push_back(b);
}

bool BookManager::remove(std::string isbn) {
  auto it = std::find_if(m_books.begin(), m_books.end(),
                      [&isbn](const Book &b) { return b.getIsbn() == isbn; });
  if (it != m_books.end()) {
    m_books.erase(it);
    return true;
  }
  return false;
}

json BookManager::log() {
  json comp;
  for (const auto &i : m_books) {
    json m = {{"title", i.getTitle()},
              {"authors", i.getAuthors()},
              {"isbn", i.getIsbn()},
              {"copies", i.getCopies()},
              {"thumbnail", i.getThumbnail()}};
    comp += m;
  }
  return comp;
}

bool BookManager::read(std::string msg) {
  try {
    json compare = json::parse(msg);
    for (const auto &comp : compare)
      this->bring({comp["title"], comp["authors"], comp["isbn"], comp["copies"],
                   comp["thumbnail"]});
    return true;
  } catch (std::exception) {
    return false;
  }
}