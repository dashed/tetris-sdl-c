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

    atexit(cleanup);

    init();

    // draw_block(9, 21, 255, 65, 54, 255);

    bool quit = false;
    while(!quit) {

        // preRender();
        // draw_playing_field();

        getInput();

        updateTetris();

        updateRender();

        // Set to ~60 fps.
        // 1000 ms/ 60 fps = 1/16 s^2/frame
        SDL_Delay(16);
    }

    return 0;
}
