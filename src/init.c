#include "init.h"

void init() {

    if(TTF_Init() == -1) {
        fprintf(stderr,
                "\nTTF_Init Error:  %s\n",
                SDL_GetError());
        exit(1);
    }

    init_graphics();

    initTetris();

}

void cleanup() {

    cleanup_graphics();

    TTF_Quit();

    // Shut down SDL
    SDL_Quit();
}
