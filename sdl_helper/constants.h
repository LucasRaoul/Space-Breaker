#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1150
#define FPS 60

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int window_width;
extern int window_height;
extern char* name;



#define MAX_AUDIO_CHANNELS 8


extern SDL_Color textColor;
extern TTF_Font* comfortaaFont_16;
extern TTF_Font* comfortaaFont_18;
extern TTF_Font* comfortaaFont_24;
extern TTF_Font* comfortaaFont_28;
extern TTF_Font* comfortaaFont_36;
extern TTF_Font* comfortaaFont_52;
extern TTF_Font* PressStart2P_16;
extern TTF_Font* PressStart2P_18;
extern TTF_Font* PressStart2P_24;
extern TTF_Font* PressStart2P_28;
extern TTF_Font* PressStart2P_36;
extern TTF_Font* PressStart2P_52;
extern TTF_Font* Orbitron_16;
extern TTF_Font* Orbitron_18;
extern TTF_Font* Orbitron_24;
extern TTF_Font* Orbitron_28;
extern TTF_Font* Orbitron_36;
extern TTF_Font* Orbitron_52;
void constantsLoadFont();
