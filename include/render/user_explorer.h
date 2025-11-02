#pragma once

#include "../core/user.h"
#include "explorer_base.h"

namespace lms {
namespace ui {

class UserExplorer : public ExplorerBase {
  public:
  UserExplorer(const std::string &title,
               const std::vector<std::unique_ptr<User>> &users)
      : ExplorerBase(title) {
    // Store pointers to users
    for (const auto &user : users) {
      this->users.push_back(user.get());
    }
    filteredUsers = this->users;
    CreateComponents();
    UpdateFilteredItems();
  }

  const User *GetSelectedUser() const { return selectedUser; }

  protected:
  std::vector<std::string> GetAllItemStrings() const override;

  const std::vector<std::string> &GetFilteredItems() const override {
    return filteredTexts;
  }

  void UpdateFilteredItems() override;

  void HandleItemSelected() override;

  void UpdateSelection(size_t index) override;

  private:
  std::vector<User *> users;
  std::vector<User *> filteredUsers;
  User *selectedUser = nullptr;

  void UpdateFilteredUsers();
};

} // namespace ui
} // namespace lms