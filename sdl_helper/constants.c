#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"


// Set window and renderer settings
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int window_width = 0;
int window_height = 0;
char* name = "SDL app";




// Set text and font settings
SDL_Color textColor = {255, 0, 0, 255};
TTF_Font* comfortaaFont_16;
TTF_Font* comfortaaFont_18;
TTF_Font* comfortaaFont_24;
TTF_Font* comfortaaFont_28;
TTF_Font* comfortaaFont_36;
TTF_Font* comfortaaFont_52;
TTF_Font* PressStart2P_16;
TTF_Font* PressStart2P_18;
TTF_Font* PressStart2P_24;
TTF_Font* PressStart2P_28;
TTF_Font* PressStart2P_36;
TTF_Font* PressStart2P_52;
TTF_Font* Orbitron_16;
TTF_Font* Orbitron_18;
TTF_Font* Orbitron_24;
TTF_Font* Orbitron_28;
TTF_Font* Orbitron_36;
TTF_Font* Orbitron_52;
void constantsLoadFont() {
    comfortaaFont_16 = TTF_OpenFont("sdl_helper/fonts/Comfortaa-Regular.ttf", 16);
    if (comfortaaFont_16 == NULL) {
        printf("Error: font loading failed. Check filepath.");
    }
    comfortaaFont_18 = TTF_OpenFont("sdl_helper/fonts/Comfortaa-Regular.ttf", 18);
    comfortaaFont_24 = TTF_OpenFont("sdl_helper/fonts/Comfortaa-Regular.ttf", 24);
    comfortaaFont_28 = TTF_OpenFont("sdl_helper/fonts/Comfortaa-Regular.ttf", 28);
    comfortaaFont_36 = TTF_OpenFont("sdl_helper/fonts/Comfortaa-Regular.ttf", 36);
    comfortaaFont_52 = TTF_OpenFont("sdl_helper/fonts/Comfortaa-Regular.ttf", 52);
    PressStart2P_16= TTF_OpenFont("sdl_helper/fonts/PressStart2P-Regular.ttf", 16);
    PressStart2P_18= TTF_OpenFont("sdl_helper/fonts/PressStart2P-Regular.ttf", 18);
    PressStart2P_28= TTF_OpenFont("sdl_helper/fonts/PressStart2P-Regular.ttf", 28);
    PressStart2P_36= TTF_OpenFont("sdl_helper/fonts/PressStart2P-Regular.ttf", 36);
    PressStart2P_52= TTF_OpenFont("sdl_helper/fonts/PressStart2P-Regular.ttf", 52);
    Orbitron_16= TTF_OpenFont("sdl_helper/fonts/Orbitron-Regular.ttf", 16);
    Orbitron_18= TTF_OpenFont("sdl_helper/fonts/Orbitron-Regular.ttf", 18);
    Orbitron_28= TTF_OpenFont("sdl_helper/fonts/Orbitron-Regular.ttf", 28);
    Orbitron_36= TTF_OpenFont("sdl_helper/fonts/Orbitron-Regular.ttf", 36);
    Orbitron_52= TTF_OpenFont("sdl_helper/fonts/Orbitron-Regular.ttf", 52);
}
