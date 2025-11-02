#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <string>

// Singleton class to manage textures
class TextureManager {
  public:
  // Get the singleton instance
  static TextureManager *Instance() {
    if (!s_pInstance) {
      s_pInstance = new TextureManager();
      return s_pInstance;
    }
    return s_pInstance;
  }

  // Load a texture from file
  bool load(std::string fileName, std::string id, SDL_Renderer *pRenderer);

  // Draw a texture
  void draw(std::string id, int x, int y, int width, int height,
            SDL_Renderer *pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

  // Draw a specific frame of a texture
  void drawFrame(std::string id, int x, int y, int width, int height,
                 int currentRow, int currentFrame, SDL_Renderer *pRenderer,
                 SDL_RendererFlip flip = SDL_FLIP_NONE);

  // Remove a texture from the map
  void clearFromTextureMap(std::string id) { m_textureMap.erase(id); }

  private:
  TextureManager() {}
  ~TextureManager() {}

  std::map<std::string, SDL_Texture *>
      m_textureMap;                   // Map of texture IDs to SDL textures
  static TextureManager *s_pInstance; // Singleton instance
};

using TheTextureManager = TextureManager;