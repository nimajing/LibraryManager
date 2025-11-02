#include "core/librarian.h"
#include "render/main_layout.h"
#include <ftxui/component/screen_interactive.hpp>

int main(int argc, char **argv) {
  if (argc > 1) {
    Librarian::Instance(argv[1]);
  }

  // Launch UI
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  lms::ui::MainLayout layout(screen);
  layout.Run();
  return 0;
}
