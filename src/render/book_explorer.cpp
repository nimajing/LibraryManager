#include "render/book_explorer.h"
#include "utils/hints.h"

namespace lms {
namespace ui {
std::vector<std::string> BookExplorer::GetAllItemStrings() const {
  std::vector<std::string> titles;
  titles.reserve(books.size());
  for (const auto &book : books) {
    titles.push_back(book.getTitle());
  }
  return titles;
}

void BookExplorer::UpdateFilteredItems() {
  auto fields = GetAllItemStrings();

  // Get filtered results
  auto result = hints(searchText, fields);
  filteredTexts = result.first;
  filteredElements = result.second;

  // Update filtered books list
  filteredBooks.clear();
  filteredBooks.reserve(filteredTexts.size());
  for (const auto &value : filteredTexts) {
    for (const auto &book : books) {
      if (book.getTitle() == value) {
        filteredBooks.push_back(book);
        break;
      }
    }
  }

  // Reset selection if it's out of bounds
  if (selectedIndex >= int(filteredTexts.size())) {
    selectedIndex = filteredTexts.empty() ? 0 : filteredTexts.size() - 1;
  }
}

void BookExplorer::HandleItemSelected() {
  if (selectedBook) {
    searchText = selectedBook->getTitle();
    cursor = searchText.size();
  }
}

void BookExplorer::UpdateSelection(size_t index) {
  if (index < filteredBooks.size()) {
    selectedBook = &filteredBooks[index];
    if (onValueChange)
      onValueChange();
  }
}

void BookExplorer::UpdateFilteredBooks() {
  std::vector<std::string> fields;
  for (const auto &book : books) {
    fields.push_back(book.getTitle());
  }
  auto result = hints(searchText, fields);
  filteredTexts = result.first;
  filteredElements = result.second;
  filteredBooks.clear();
  for (const auto &value : filteredTexts) {
    for (const auto &book : books) {
      if (book.getTitle() == value) {
        filteredBooks.push_back(book);
        break;
      }
    }
  }
}

} // namespace ui
} // namespace lms