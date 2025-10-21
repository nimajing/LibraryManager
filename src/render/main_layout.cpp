#include "../include/render/main_layout.h"
#include "core/librarian.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace lms {
namespace ui {

MainLayout::MainLayout(ftxui::ScreenInteractive &screen) : screen(screen) {
  // Get data from librarian
  auto librarian = Librarian::Instance();

  // Initialize components
  userExplorer = std::make_unique<UserExplorer>("Users", librarian->getUsers());
  bookExplorer = std::make_unique<BookExplorer>("Books", librarian->getBooks());
  editor = std::make_unique<Editor>();
  summary = std::make_unique<Summary>();

  // Set up callbacks
  userExplorer->SetOnValueChange([this]() { HandleUserSelection(); });
  bookExplorer->SetOnValueChange([this]() { HandleBookSelection(); });

  CreateLayout();
}

void MainLayout::CreateLayout() {
  using namespace ftxui;

  // Create the main horizontal container for the top row
  auto topRow = Container::Horizontal(
      {userExplorer->Render(), editor->Render(), bookExplorer->Render()});

  // Create the main vertical container for the entire layout
  mainContainer = Container::Vertical({topRow, summary->Render()});

  // Add event handling
  mainContainer = CatchEvent(mainContainer, [&](Event event) {
    if (event == Event::Escape) {
      Exit();
      return true;
    }
    return false;
  });
}

void MainLayout::HandleUserSelection() {
  if (const User *user = userExplorer->GetSelectedUser()) {
    std::string summaryText = "Username: " + user->getUsername() + "\n";
    summaryText += "Thumbnail: " + user->getThumbnail() + "\n";
    // Add more user details as needed

    summary->SetContent(summaryText);
    editor->SetMode(Editor::Mode::EDIT_USER);
    // TODO: Populate editor fields with user data
    editor->user.username = user->getUsername();
    editor->user.thumbnail = user->getThumbnail();
  }
}

void MainLayout::HandleBookSelection() {
  if (const Book *book = bookExplorer->GetSelectedBook()) {
    std::string summaryText = "Title: " + book->getTitle() + "\n";
    summaryText += "Authors: ";
    for (size_t i = 0; i < book->getAuthors().size(); ++i) {
      summaryText += book->getAuthors()[i];
      summaryText += (i < book->getAuthors().size() - 1 ? ", " : "\n");
    }
    summaryText += "ISBN: " + book->getIsbn() + "\n";
    // Add more book details as needed

    summary->SetContent(summaryText);
    editor->SetMode(Editor::Mode::EDIT_BOOK);
    // TODO: Populate editor fields with book data
    editor->book.title = book->getTitle();
    editor->book.isbn = book->getIsbn();
    editor->book.authors = book->getAuthors();
    editor->book.copies = std::to_string(book->getCopies());
    editor->book.thumbnail = book->getThumbnail();
  }
}

void MainLayout::Run() { screen.Loop(mainContainer); }

void MainLayout::Exit() { screen.ExitLoopClosure()(); }

} // namespace ui
} // namespace lms
