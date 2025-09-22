#include "ftxui/dom/node.hpp"

int levenshtein(std::string a, std::string b);

std::pair<std::vector<std::string>, std::vector<ftxui::Element>>
hints(const std::string &input, const std::vector<std::string> &suggestions);