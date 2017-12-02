#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include "sds.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2_gfxPrimitives.h"

#ifndef _GLOBAL_CONSTANTS
#define _GLOBAL_CONSTANTS

#define WINDOW_TITLE "tetris-sdl-c"

// a block 'pixel' of a playing field is 15px by 15px in size
#define BLOCK_SIZE 20

// standard size of a tetris playing field
#define PLAYFIELD_HEIGHT 22
#define PLAYFIELD_WIDTH 10

#define WINDOW_HEIGHT PLAYFIELD_HEIGHT * (BLOCK_SIZE + 1) + 1
#define WINDOW_WIDTH PLAYFIELD_WIDTH * (BLOCK_SIZE + 1) + 1

#endif
