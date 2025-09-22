#include "../include/render/editor.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace lms {
namespace ui {

Editor::Editor() { CreateComponents(); }

void Editor::CreateComponents() {
  // Create a container for the form fields
  // Initialize a stable proxy child that will forward to the live `container`.
  using namespace ftxui;
  proxy_child = Renderer([this] {
    // If container is null, render an empty placeholder.
    if (!container)
      return vbox({});
    return container->Render();
  });

  // Update the layout to populate `container` and `inputs`.
  UpdateLayout();
}

void Editor::UpdateLayout() {
  using namespace ftxui;

  // Properly detach all existing components
  if (container) {
    container->OnEvent(ftxui::Event::Special("Escape"));
    for (auto &input : inputs) {
      input->OnEvent(ftxui::Event::Special("Escape"));
    }
  }

  // Clear existing components
  inputs.clear();
  container = nullptr;

  std::vector<Component> newInputs;

  // Add a submit button
  auto submitButton = Button("Submit", [this] {
    if (onValueChange)
      onValueChange();
  });

  switch (currentMode) {
  case Mode::ADD_BOOK:
  case Mode::EDIT_BOOK: {
    newInputs.push_back(Input(&title, "Title"));
    newInputs.push_back(Input(&author, "Author"));
    newInputs.push_back(Input(&isbn, "ISBN"));
    newInputs.push_back(submitButton);
    break;
  }
  case Mode::ADD_USER:
  case Mode::EDIT_USER: {
    newInputs.push_back(Input(&name, "Name"));
    newInputs.push_back(Input(&email, "Email"));
    newInputs.push_back(Input(&id, "ID"));
    newInputs.push_back(submitButton);
    break;
  }
  case Mode::SELECT: {
    auto addUserBtn = Button("-> Add user", [this] {
      this->container = nullptr;
      this->inputs.clear();
      SetMode(Mode::ADD_USER);
    });
    auto addBookBtn = Button("-> Add book", [this] {
      this->container = nullptr;
      this->inputs.clear();
      SetMode(Mode::ADD_BOOK);
    });
    newInputs.push_back(addUserBtn);
    newInputs.push_back(addBookBtn);
    break;
  }
  }

  // Create new container and store components
  inputs = newInputs;
  container = Container::Vertical(std::move(newInputs));

  // Ensure the proxy child will delegate to the new container (no-op: proxy
  // reads container each render)
}

ftxui::Component Editor::Render() {
  using namespace ftxui;
  // Use a Renderer with a stable child (proxy_child) that delegates to
  // `container` so FTXUI event routing knows where to dispatch events and we
  // don't hold stale children.
  return Renderer(proxy_child, [this] {
    std::string title;
    switch (currentMode) {
    case Mode::ADD_BOOK:
      title = "Add New Book";
      break;
    case Mode::EDIT_BOOK:
      title = "Edit Book";
      break;
    case Mode::ADD_USER:
      title = "Add New User";
      break;
    case Mode::EDIT_USER:
      title = "Edit User";
      break;
    case Mode::SELECT:
      title = "Select action";
      break;
    }

    Elements inputElements;
    for (const auto &input : inputs) {
      inputElements.push_back(input->Render());
      inputElements.push_back(separator());
    }

    return window(text(title) | hcenter, vbox(std::move(inputElements)) | flex);
  });
}

void Editor::SetMode(Mode mode) {
  currentMode = mode;
  // Clear focus before updating layout
  if (container) {
    container->TakeFocus();
    container->OnEvent(ftxui::Event::Special("Escape"));
  }
  UpdateLayout();
  // Set focus to the first element of the new container
  if (container) {
    container->TakeFocus();
  }
}

void Editor::Reset() { UpdateLayout(); }

} // namespace ui
} // namespace lms