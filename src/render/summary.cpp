#include "../include/render/summary.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace lms {
namespace ui {

Summary::Summary() { CreateComponents(); }

void Summary::CreateComponents() {
  using namespace ftxui;
  display = Renderer([this] {
    return window(text("Summary") | hcenter, paragraph(content) | flex);
  });
}

ftxui::Component Summary::Render() { return display; }

void Summary::SetContent(const std::string &newContent) {
  content = newContent;
  if (onValueChange)
    onValueChange();
}

} // namespace ui
} // namespace lms