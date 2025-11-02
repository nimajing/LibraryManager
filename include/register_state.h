// This state handles the registration of the user
// It is called through the login state when the user presses the register
// button It is also called through the main menu when the user presses the
// register button

#ifndef REGISTER_STATE_H
#define REGISTER_STATE_H

#include "manager.h"
#include "state.h"
#include "user.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class RegisterState : public State {
  private:
  // Window dimensions
  static const int WINDOW_WIDTH = 400;
  static const int WINDOW_HEIGHT = 500;

  // Form input fields
  std::string m_inputUsername;
  std::string m_inputPassword;
  std::string m_inputConfirmPassword;
  std::string m_inputFullName;
  std::string m_inputEmail;

  // Form input field focus
  bool m_focusUsername;
  bool m_focusPassword;
  bool m_focusConfirmPassword;
  bool m_focusFullName;
  bool m_focusEmail;

  // SDL Window components
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  TTF_Font *m_font;
  bool m_running;

  // Input field textures
  SDL_Texture *m_usernameTexture;
  SDL_Texture *m_passwordTexture;
  SDL_Texture *m_confirmPassTexture;
  SDL_Texture *m_fullNameTexture;
  SDL_Texture *m_emailTexture;

  // Button components
  Button m_submitButton;
  Button m_backButton;
  Button m_clearButton;

  // Input field positions and dimensions
  SDL_Rect m_usernameRect;
  SDL_Rect m_passwordRect;
  SDL_Rect m_confirmPassRect;
  SDL_Rect m_fullNameRect;
  SDL_Rect m_emailRect;

  // Label positions and dimensions
  SDL_Rect m_usernameLabelRect;
  SDL_Rect m_passwordLabelRect;
  SDL_Rect m_confirmPassLabelRect;
  SDL_Rect m_fullNameLabelRect;
  SDL_Rect m_emailLabelRect;

  // Key state tracking
  bool m_tab_lock;
  bool m_back_lock;

  // Form state tracking
  bool m_isProcessing;
  bool m_hasError;
  std::string m_errorMessage;

  // Helper methods
  bool validateInput() const;
  bool createNewUser();
  void clearForm();
  void centerWindow();
  void initializeRects();
  SDL_Texture *createTextTexture(const std::string &text);

  public:
  RegisterState();
  ~RegisterState();

  SDL_Window *getWindow() const { return m_window; }

  void onEnter() override;
  void onExit() override;
  void update() override;
  void render() override;
};

#endif // REGISTER_STATE_H