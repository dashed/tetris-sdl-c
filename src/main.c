#include "main.h"

int main(int argc, const char *argv[]) {

    // Start up SDL, and make sure it went ok
    //
    // TODO: change from SDL_INIT_EVERYTHING down the road
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

        fprintf(stderr,
                "\nUnable to initialize SDL:  %s\n",
                SDL_GetError());

        return 1;
    }

    init();

    atexit(cleanup);

    bool quit = false;
    while(!quit) {
        getInput();

        updateRender();

        // Sleep briefly to stop sucking up all the CPU time
        // 1000 ms/ 60 fps = 1/16 s^2/frame
        SDL_Delay(16);
    }

    return 0;
}
