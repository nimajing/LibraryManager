#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include <algorithm>

int levenshtein(std::string a, std::string b) {
  std::vector<std::vector<int>> grid(a.length() + 1,
                                     std::vector<int>(b.length() + 1));
  for (size_t i = 0; i <= a.length(); ++i)
    grid[i][0] = i;
  for (size_t i = 0; i <= b.length(); ++i)
    grid[0][i] = i;
  auto min = [](int a, int b, int c) {
    auto t = [](int a, int b) { return a < b ? a : b; };
    return t(t(a, b), c);
  };
  for (size_t i = 1; i <= a.length(); ++i) {
    for (size_t j = 1; j <= b.length(); ++j) {
      grid[i][j] =
          a[i - 1] == b[j - 1]
              ? grid[i - 1][j - 1]
              : min(grid[i - 1][j], grid[i][j - 1], grid[i - 1][j - 1]) + 1;
    }
  }
  return grid[a.length()][b.length()];
}

std::pair<std::vector<std::string>, std::vector<ftxui::Element>>
hints(const std::string &input, const std::vector<std::string> &suggestions) {
  using namespace ftxui;
  std::vector<std::string> filtered_texts;
  std::vector<Component> filtered_suggestions;
  if (suggestions.empty())
    return {};
  // Create a lowercase copy of input for matching
  std::string input_lower = input;
  std::transform(input_lower.begin(), input_lower.end(), input_lower.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  for (auto &suggestion : suggestions) {
    bool present = false;
    // Do not modify the original suggestion string in-place because
    // `suggestions` is a const reference. Create a lowercase copy for
    // matching only and keep the original for display.
    std::string lower = suggestion;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (lower.find(input_lower) == 0) {
      present = true;
    } else if (levenshtein(input_lower, lower) <=
               int(input_lower.length() / 2)) {
      present = true;
    }
    if (!present)
      continue;
    // Build an Element directly (avoid creating Renderer lambdas that
    // capture loop variables by reference). Highlight characters that
    // appear in the (lowercased) input.
    std::vector<Element> chars;
    for (size_t i = 0; i < suggestion.length(); ++i) {
      char display_char = suggestion[i];
      char lower_char = lower[i];
      bool highlight =
          (input.find(std::string(1, lower_char)) != std::string::npos);
      if (highlight)
        chars.push_back(text(std::string(1, display_char)) | bold);
      else
        chars.push_back(text(std::string(1, display_char)));
    }
    Component text = Container::Horizontal({});
    // Create a small component only to follow previous API; Render into
    // an Element below. Add each char as a small renderer component.
    for (auto &e : chars) {
      // Wrap each Element in a Renderer so we can keep the same
      // Component-based structure as before.
      auto c = Renderer([e] { return e; });
      text->Add(c);
    }
    filtered_suggestions.push_back(text);
    filtered_texts.push_back(suggestion);
  }

  std::vector<Element> suggested_elements;
  for (size_t i = 0; i < filtered_suggestions.size(); ++i) {
    Element e(filtered_suggestions[i]->Render());
    suggested_elements.push_back(e);
  }
  return {filtered_texts, suggested_elements};
}