#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "icon.h"

// Function to load an icon from a PNG file
int load_png_icon(const char* filepath, Icon* icon);

int main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <png file>\n", argv[0]);
    return -1;
  }

  // Load the icon
  Icon icon;
  if (load_png_icon(argv[1], &icon) < 0) {
    return -1;
  }

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return -1;
  }

  // Create the window
  SDL_Window* window = SDL_CreateWindow("Icon Test",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					128, 128, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  // Create the renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  // Create the texture
  SDL_Texture* texture = SDL_CreateTexture(renderer,
					   SDL_PIXELFORMAT_ARGB8888,
					   SDL_TEXTUREACCESS_STREAMING,
					   128, 128);
  if (texture == NULL) {
    fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }   
  // Update the texture with the icon data
  SDL_UpdateTexture(texture, NULL, icon.pixels, 128 * sizeof(uint32_t));

  // Clear the renderer
  SDL_RenderClear(renderer);

  // Render the texture
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  // Wait for the user to close the window
  SDL_Event event;
  while (SDL_WaitEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      break;
    }
  }

  // Clean up
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
