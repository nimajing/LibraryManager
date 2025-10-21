#include "core/librarian.h"
#include "render/main_layout.h"
#include <ftxui/component/screen_interactive.hpp>

int main(int argc, char** argv) {
	Librarian::Instance("assets/program_data/users.json");
 // Launch UI
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  lms::ui::MainLayout layout(screen);
  layout.Run();
  return 0;
}
