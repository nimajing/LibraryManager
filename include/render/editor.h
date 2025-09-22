#pragma once

#include "ui_component.h"
#include <vector>

namespace lms {
namespace ui {

class Editor : public UIComponent {
public:
  enum class Mode { SELECT, ADD_BOOK, EDIT_BOOK, ADD_USER, EDIT_USER };

  Editor();
  ftxui::Component Render() override;
  void SetMode(Mode mode);
  void Reset();
  void setReadOnly() { readonly = true; }
  std::string title, author, isbn, name, email, id;
  std::string username, thumbnail;

private:
  Mode currentMode = Mode::SELECT;
  std::vector<ftxui::Component> inputs;
  ftxui::Component container;
  // A stable proxy child held by the Renderer which delegates to `container`.
  ftxui::Component proxy_child;
  bool readonly;

  // Persistent input fields
  void CreateComponents();
  void UpdateLayout();
};

} // namespace ui
} // namespace lms