#pragma once

#include "book_explorer.h"
#include "editor.h"
#include "summary.h"
#include "user_explorer.h"


namespace lms {
namespace ui {

class MainLayout {
public:
  MainLayout(ftxui::ScreenInteractive &screen);
  void Run();
  void Exit();

private:
  ftxui::ScreenInteractive &screen;
  std::unique_ptr<UserExplorer> userExplorer;
  std::unique_ptr<BookExplorer> bookExplorer;
  std::unique_ptr<Editor> editor;
  std::unique_ptr<Summary> summary;
  ftxui::Component mainContainer;

  void CreateLayout();
  void HandleUserSelection();
  void HandleBookSelection();
  void UpdateSummary();
};

} // namespace ui
} // namespace lms