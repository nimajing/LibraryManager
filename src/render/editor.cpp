#include "../include/render/editor.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace lms {
namespace ui {

Editor::Editor() { CreateComponents(); }

void Editor::CreateComponents() {
  using namespace ftxui;
  UpdateLayout();
}

void Editor::UpdateLayout() {
  using namespace ftxui;
  if(!container) container = Container::Vertical({});
  inputs.clear();

  // Add a submit button
  auto submitButton = Button("Submit", [this] {
    if (onValueChange)
      onValueChange();
  });

  switch (currentMode) {
  case Mode::ADD_BOOK:
    title = "Add book";
  case Mode::EDIT_BOOK:
    title = "Edit book";
    inputs.push_back(Input(&book.title, "Title"));
    for(size_t i = 0; i < book.authors.size(); ++i) {
      inputs.push_back(Input(&book.authors[i], std::string("Author ") + std::to_string(i)));
    }
    inputs.push_back(Input(&book.isbn, "ISBN"));
    inputs.push_back(Input(&book.copies, "Copies"));
    inputs.push_back(Input(&book.thumbnail, "Thumbnail"));
    inputs.push_back(submitButton);
    break;
  case Mode::ADD_USER:
    title = "Add User";
  case Mode::EDIT_USER:
    title = "Edit User";
    inputs.push_back(Input(&user.username, "Name"));
    inputs.push_back(Input(&user.thumbnail, "Thumbnail"));
    inputs.push_back(submitButton);
    break;
  case Mode::SELECT:
    title = "Select Mode";
    inputs.push_back(Button("-> Add user", [this] {
          SetMode(Mode::ADD_USER);
    }));
    inputs.push_back(Button("-> Add book", [this] {
          SetMode(Mode::ADD_BOOK);
    }));
    break;
  }

  for(const auto& i : inputs)
    container->Add(i);
}

ftxui::Component Editor::Render() {
  using namespace ftxui;

  return Renderer(container, [this] {
    Elements inputElements;
    for(const auto& i : inputs) inputElements.push_back(i->Render());
    return window(text(title) | hcenter, vbox(inputElements) | flex);
  });
}

void Editor::SetMode(Mode mode) {
  currentMode = mode;
  UpdateLayout();
}

void Editor::Reset() { UpdateLayout(); }

} // namespace ui
} // namespace lms
