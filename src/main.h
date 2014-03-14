#include "defs.h"

#include "init.h"
#include "graphics.h"
#include "input.h"
#include "tetris.h"

// extern vars from graphics.h
SDL_Window *window;
SDL_Renderer *render;
SDL_Texture *display;

bool render_changed;
