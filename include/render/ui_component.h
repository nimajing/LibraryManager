#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <functional>


namespace lms {
namespace ui {

class UIComponent {
public:
  virtual ~UIComponent() = default;
  virtual ftxui::Component Render() = 0;
  virtual void SetOnValueChange(std::function<void()> callback) {
    onValueChange = callback;
  }

protected:
  std::function<void()> onValueChange = []() {};
};

} // namespace ui
} // namespace lms