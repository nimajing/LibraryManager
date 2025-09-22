
#include "core/book.h"
#include "core/librarian.h"
#include "render/main_layout.h"
#include <ftxui/component/screen_interactive.hpp>

int main() {
  // Create dummy users
  auto librarian = Librarian::Instance();
  std::vector<std::string> usernames = {"alice", "bob", "carol", "dave", "eve"};
  for (int i = 0; i < 5; ++i) {
    librarian->addUser(usernames[i]);
  }

  // Create dummy books
  std::vector<Book> books = {
      Book("The Great Gatsby", {"F. Scott Fitzgerald"}, "9780743273565"),
      Book("1984", {"George Orwell"}, "9780451524935"),
      Book("To Kill a Mockingbird", {"Harper Lee"}, "9780061120084"),
      Book("Pride and Prejudice", {"Jane Austen"}, "9780141439518"),
      Book("Moby-Dick", {"Herman Melville"}, "9781503280786")};
  for (const auto &book : books) {
    librarian->addBook(book);
  }

  // Launch UI
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  lms::ui::MainLayout layout(screen);
  layout.Run();
  return 0;
}