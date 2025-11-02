#pragma once

#include "ui_component.h"
#include <string>

namespace lms {
namespace ui {

class Summary : public UIComponent {
  public:
  Summary();
  ftxui::Component Render() override;
  void SetContent(const std::string &content);

  private:
  std::string content;
  ftxui::Component display;
  void CreateComponents();
};

} // namespace ui
} // namespace lms