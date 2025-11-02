// singleton class that puts together all concepts of the Library Management
// System It is responsible for managing the library, the users, the books, the
// transactions, etc. It is the main class that interacts with the user
// interface and the database It is the class that is responsible for the main
// functionality of the system It must load, update, render and save all the
// data of the system upon exiting. It is also responsible for rendering the ui
// and updating the ui It uses the SDL2 library

#pragma once

#include "input_handler.h"
#include "state_machine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// enum for storing different error codes
enum Error {
  SUCCESS = 0,
  SDL_INIT_FAIL = 1,
  TTF_INIT_FAIL = 2,
  WINDOW_CREATION_ERROR = 3,
  RENDERER_CREATION_ERROR = 4,
  AUTHENTICATION_FAILED = 5
};

struct Button {
  int x, y, w, h;
  std::string text;
  SDL_Color textColor;
  SDL_Color buttonColor;
  SDL_Color hoverColor;
  SDL_Color clickColor;

  Uint32 clickTime; // Time when the button was clicked
  bool clicked;     // Indicates if the button was clicked

  Button() {
    // Enhanced colors with better contrast
    textColor = {0, 0, 0, 255};         // Black text
    buttonColor = {240, 240, 240, 255}; // Very light grey base
    hoverColor = {200, 200, 200, 255};  // Medium grey for hover
    clickColor = {160, 160, 160, 255};  // Darker grey for click
    clicked = false;
    clickTime = 0;
  }

  void setSize(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
  }

  void setText(std::string text) { this->text = text; }

  // Render the button
  void render(SDL_Renderer *renderer, const char *font, bool isHovered,
              bool isClicked) {
    SDL_Color currentColor = buttonColor;

    // Determine button color state
    if (isClicked || clicked) {
      currentColor = clickColor;
    } else if (isHovered) {
      currentColor = hoverColor;
    }

    // Draw button shadow (3D effect)
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect shadowRect = {x + 2, y + 2, w, h};
    SDL_RenderFillRect(renderer, &shadowRect);

    // Draw button background
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g,
                           currentColor.b, currentColor.a);
    SDL_Rect buttonRect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &buttonRect);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &buttonRect);

    // Add highlight effect on hover
    if (isHovered && !isClicked && !clicked) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
      SDL_Rect highlightRect = {x + 2, y + 2, w - 4, 2};
      SDL_RenderFillRect(renderer, &highlightRect);
    }

    // Render button text
    TTF_Font *ttfFont = TTF_OpenFont(font, 24);
    if (ttfFont) {
      // Add slight text offset when clicked
      int textOffsetX = (isClicked || clicked) ? 1 : 0;
      int textOffsetY = (isClicked || clicked) ? 1 : 0;

      SDL_Surface *textSurface =
          TTF_RenderText_Solid(ttfFont, text.c_str(), textColor);
      if (textSurface) {
        SDL_Texture *textTexture =
            SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {x + (w - textSurface->w) / 2 + textOffsetX,
                             y + (h - textSurface->h) / 2 + textOffsetY,
                             textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
      }
      TTF_CloseFont(ttfFont);
    }
  }

  // Check if a point is inside the button (for click detection)
  bool isOver(int mouseX, int mouseY) const {
    return mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h;
  }

  void click() { clicked = true; }

  // Handle click effect duration
  void handleClickEffect(Uint32 currentTime) {
    if (clicked && currentTime - clickTime >=
                       100) // Increased from 50ms for better visibility
    {
      clicked = false; // Reset clicked state after the delay
    }
  }
};

class Manager {
  private:
  Manager();

  // keep window running
  bool m_isRunning;

  // Window dimensions
  int m_width;
  int m_height;

  // State manager
  StateMachine *m_pStateMachine;

  // SDL render variables
  SDL_Window *m_pWindow;
  SDL_Renderer *m_pRenderer;

  static Manager *s_pInstance;

  public:
  static Manager *Instance() {
    if (s_pInstance == 0) {
      s_pInstance = new Manager();
    }
    return s_pInstance;
  }

  void stop() { m_isRunning = false; }
  void renderText(const char *text, int x, int y, int pxSize, const char *font,
                  SDL_Color color = {0, 0, 0, 255}, bool showCursor = false,
                  bool rightAlign = false);
  void renderBox(int x, int y, int w, int h, SDL_Color color = {0, 0, 0, 255});

  void resize(int width, int height);

  // getters
  SDL_Renderer *getRenderer() const { return m_pRenderer; }
  StateMachine *getStateMachine() const { return m_pStateMachine; }
  int getWidth() { return m_width; }
  int getHeight() { return m_height; }
  int getTextWidth(const char *text, int pxSize, const char *font);
  bool isRunning() const { return m_isRunning; }

  ~Manager() {}
  Error init(const char *title, int x, int y, int w, int h, int flags = 0);
  void update();
  void render();
  void run();
  void clean();
};