#pragma once

#include "../core/book.h"
#include "explorer_base.h"

namespace lms {
namespace ui {

class BookExplorer : public ExplorerBase {
  public:
  enum class SearchCriteria { TITLE, AUTHOR, ISBN };

  BookExplorer(const std::string &title, const std::vector<Book> &books)
      : ExplorerBase(title), books(books), filteredBooks(books) {
    CreateComponents();
    UpdateFilteredItems();
  }

  const Book *GetSelectedBook() const { return selectedBook; }

  protected:
  std::vector<std::string> GetAllItemStrings() const override;

  const std::vector<std::string> &GetFilteredItems() const override {
    return filteredTexts;
  }

  void UpdateFilteredItems() override;

  void HandleItemSelected() override;

  void UpdateSelection(size_t index) override;

  private:
  std::vector<Book> books;
  std::vector<Book> filteredBooks;
  const Book *selectedBook = nullptr;

  void UpdateFilteredBooks();
};

} // namespace ui
} // namespace lms