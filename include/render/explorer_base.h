#pragma once

#include "ui_component.h"
#include <string>
#include <vector>

namespace lms {
namespace ui {

class ExplorerBase : public UIComponent {
  public:
  ExplorerBase(const std::string &title)
      : title(title), selectedIndex(0), cursor(0) {}

  virtual ~ExplorerBase() = default;

  ftxui::Component Render() override;

  void SetSearchFilter(const std::string &filter) {
    searchText = filter;
    UpdateFilteredItems();
  }

  protected:
  // Common member variables
  std::string title;
  std::string searchText;
  ftxui::Component searchInput;
  ftxui::Component list;
  std::vector<std::string> filteredTexts;
  std::vector<ftxui::Element> filteredElements;
  int selectedIndex;
  int cursor;

  // Virtual methods to be implemented by derived classes
  virtual std::vector<std::string> GetAllItemStrings() const = 0;
  virtual const std::vector<std::string> &GetFilteredItems() const = 0;
  virtual void UpdateFilteredItems() = 0;
  virtual void HandleItemSelected() = 0;
  virtual void UpdateSelection(size_t index) = 0;

  void CreateComponents();
};

} // namespace ui
} // namespace lms