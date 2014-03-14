#include "main.h"

int main(int argc, const char *argv[]) {

    // Start up SDL, and make sure it went ok
    //
    uint32_t flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

        fprintf(stderr,
                "\nUnable to initialize SDL:  %s\n",
                SDL_GetError());

        return 1;
    }

    atexit(cleanup);

    init();

    bool quit = false;
    while(!quit) {

        preRender();

        getInput();

        updateTetris();

        postRender();

        updateRender();

        // Set to ~60 fps.
        // 1000 ms/ 60 fps = 1/16 s^2/frame
        SDL_Delay(16);
    }

    return 0;
}
