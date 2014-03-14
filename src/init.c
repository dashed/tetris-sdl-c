#include "init.h"

void init() {

    init_graphics();

    initTetris();

}

void cleanup() {

    cleanup_graphics();

    // Shut down SDL
    SDL_Quit();
}
