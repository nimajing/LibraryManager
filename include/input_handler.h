// Singleton class to handle input events
// called once every frame to update the input state

#pragma once

#include <SDL2/SDL_video.h>
#include "vector2D.h"
#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

struct WindowInputState {
    Uint8* keystates;
    std::map<Uint8, bool> mouseButtons;
    Vector2D* mousePos;
    std::string text;

    WindowInputState() : 
        keystates(nullptr),
        mousePos(new Vector2D(0, 0)),
        text("") {
        mouseButtons[SDL_BUTTON_LEFT] = false;
        mouseButtons[SDL_BUTTON_MIDDLE] = false;
        mouseButtons[SDL_BUTTON_RIGHT] = false;
    }

    ~WindowInputState() {
        delete mousePos;
        delete[] keystates;
    }
};

class InputHandler {
    private:
        InputHandler();
        static InputHandler* s_pInstance;
        
        std::unordered_map<Uint32, WindowInputState*> m_windowStates;
        SDL_Window* m_currentWindow;
        
    public:
        static InputHandler* Instance() {
            if(!s_pInstance) {
                s_pInstance = new InputHandler();
                return s_pInstance;
            }
            return s_pInstance;
        }
        
        void setCurrentWindow(SDL_Window* window);
        bool isKeyPressed(const SDL_Scancode &key) const;
        bool isMouseButtonPressed(const Uint8& button) const;
        Vector2D* getMousePosition() const;
        std::string getText();
        void clearText();
        
        void update();
        void clean();
        void cleanWindow(SDL_Window* window);
};