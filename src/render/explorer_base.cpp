#include "render/explorer_base.h"

namespace lms {
namespace ui {
ftxui::Component ExplorerBase::Render() {
  auto mainContainer = ftxui::Container::Vertical({searchInput, list});

  // Add event handling at the container level
  mainContainer |= ftxui::CatchEvent([this](ftxui::Event event) {
    if (searchInput->Focused() || list->Focused()) {
      // Handle return key for selection
      if (event == ftxui::Event::Return) {
        if (!GetFilteredItems().empty()) {
          UpdateSelection(selectedIndex);
          HandleItemSelected();
          return true;
        }
      }

      // Handle arrow keys for list navigation
      if (GetFilteredItems().empty())
        return false;

      if (event == ftxui::Event::ArrowDown) {
        if (selectedIndex < int(filteredTexts.size()) - 1) {
          ++selectedIndex;
        } else {
          selectedIndex = 0;
        }
        return true;
      }

      if (event == ftxui::Event::ArrowUp) {
        if (selectedIndex > 0) {
          --selectedIndex;
        } else {
          selectedIndex = filteredTexts.size() - 1;
        }
        return true;
      }

      return false;
    }
    return false;
  });

  return Renderer(mainContainer, [this] {
    return window(
        ftxui::text(title) | ftxui::hcenter,
        ftxui::vbox(
            {hbox(ftxui::text(L"🔍 "), searchInput->Render()) |
                 size(ftxui::WIDTH, ftxui::EQUAL, 25),
             ftxui::separator(),
             list->Render() | ftxui::vscroll_indicator | ftxui::frame}));
  });
}

void ExplorerBase::CreateComponents() {
  // Create search input
  ftxui::InputOption op;
  op.cursor_position = &cursor;
  searchInput = Input(&searchText, "Search...", op);

  // Initial filtering
  UpdateFilteredItems();

  // Create list component
  list = ftxui::Renderer([this] {
    ftxui::Elements elements;
    for (size_t i = 0; i < filteredElements.size(); ++i) {
      ftxui::Element e = filteredElements[i];
      if (selectedIndex == int(i))
        e |= ftxui::bgcolor(ftxui::Color::GrayDark) | ftxui::inverted;
      elements.push_back(e);
    }
    return vbox(std::move(elements));
  });

  // Update filtered items when search text changes
  searchInput |= ftxui::CatchEvent([this](ftxui::Event event) {
    if (event.is_character() || event == ftxui::Event::Delete ||
        event == ftxui::Event::Backspace) {
      if (event.is_character()) {
        searchText =
            searchText.substr(0, cursor) + event.character() +
            (cursor == int(searchText.size()) ? "" : searchText.substr(cursor));
        ++cursor;
      }
      if (event == ftxui::Event::Backspace && !searchText.empty() &&
          cursor > 0) {
        searchText =
            searchText.substr(0, cursor - 1) + searchText.substr(cursor);
        cursor = std::max(0, cursor - 1);
      }
      if (event == ftxui::Event::Delete) {
        if (cursor < int(searchText.size())) {
          searchText =
              searchText.substr(0, cursor) + searchText.substr(cursor + 1);
        }
      }
      UpdateFilteredItems();
      return true;
    }
    return false;
  });
}

} // namespace ui
} // namespace lms