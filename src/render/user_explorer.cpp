#include "render/user_explorer.h"
#include "utils/hints.h"

namespace lms {
namespace ui {

std::vector<std::string> UserExplorer::GetAllItemStrings() const {
  std::vector<std::string> usernames;
  usernames.reserve(users.size());
  for (const auto *user : users) {
    usernames.push_back(user->getUsername());
  }
  return usernames;
}

void UserExplorer::UpdateFilteredItems() {
  auto usernames = GetAllItemStrings();

  // Get filtered results
  auto result = hints(searchText, usernames);
  filteredTexts = result.first;
  filteredElements = result.second;

  // Update filtered users list
  filteredUsers.clear();
  filteredUsers.reserve(filteredTexts.size());
  for (const auto &name : filteredTexts) {
    for (auto *user : users) {
      if (user->getUsername() == name) {
        filteredUsers.push_back(user);
        break;
      }
    }
  }

  // Reset selection if it's out of bounds
  if (selectedIndex >= int(filteredTexts.size())) {
    selectedIndex = filteredTexts.empty() ? 0 : filteredTexts.size() - 1;
  }
}

void UserExplorer::HandleItemSelected() {
  if (selectedUser) {
    searchText = selectedUser->getUsername();
    cursor = searchText.size();
  }
}

void UserExplorer::UpdateSelection(size_t index) {
  if (index < filteredUsers.size()) {
    selectedUser = filteredUsers[index];
    if (onValueChange)
      onValueChange();
  }
}

void UserExplorer::UpdateFilteredUsers() {
  std::vector<std::string> usernames;
  usernames.reserve(users.size());
  for (const auto *user : users) {
    usernames.push_back(user->getUsername());
  }

  // Get filtered results
  auto result = hints(searchText, usernames);
  filteredTexts = result.first;
  filteredElements = result.second;

  // Update filtered users list
  filteredUsers.clear();
  filteredUsers.reserve(filteredTexts.size());
  for (const auto &name : filteredTexts) {
    for (auto *user : users) {
      if (user->getUsername() == name) {
        filteredUsers.push_back(user);
        break;
      }
    }
  }

  // Reset selection if it's out of bounds
  if (selectedIndex >= int(filteredUsers.size())) {
    selectedIndex = filteredUsers.empty() ? 0 : filteredUsers.size() - 1;
  }
}

} // namespace ui
} // namespace lms