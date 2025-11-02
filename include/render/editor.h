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
  struct {
    std::string title, isbn, thumbnail, copies;
    int author;
    bool authors_dropdown;
    std::vector<std::string> authors = {};
  } book;
  struct {
    std::string username, thumbnail;
  } user;

  private:
  Mode currentMode = Mode::SELECT;
  std::vector<ftxui::Component> inputs;
  std::string title;
  ftxui::Component container;
  bool readonly;

  void CreateComponents();
  void UpdateLayout();
};

} // namespace ui
} // namespace lms
