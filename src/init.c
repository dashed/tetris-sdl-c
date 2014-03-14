#include "init.h"

// TODO: Put this somewhere else
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

void init(char * window_title) {

    window = SDL_CreateWindow(
        // title of window
        window_title,

        // initial position of the window
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,

        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr,
                "\nSDL_CreateWindow Error:  %s\n",
                SDL_GetError());
        exit(1);
    }

    // Create a renderer that will draw to the window, -1 specifies that we want to load whichever
    // video driver supports the flags we're passing
    //
    // Flags:
    // SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
    // SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
    // synchornized with the monitor's refresh rate
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (render == NULL) {
        fprintf(stderr,
                "\nSDL_CreateRenderer Error:  %s\n",
                SDL_GetError());
        exit(1);
    }


    //SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
    //this lets us choose when to upload or remove textures from the GPU
    SDL_Surface *bmp = SDL_LoadBMP("src/res/hello.bmp");
    if (bmp == NULL){

        fprintf(stderr,
                "\nSDL_LoadBMP Error:  %s\n",
                SDL_GetError());
        exit(1);
    }

    //To use a hardware accelerated texture for rendering we can create one from
    //the surface we loaded
    SDL_Texture *tex = SDL_CreateTextureFromSurface(render, bmp);
    //We no longer need the surface
    SDL_FreeSurface(bmp);
    if (tex == NULL){

        fprintf(stderr,
                "\nSDL_CreateTextureFromSurface Error:  %s\n",
                SDL_GetError());
        exit(1);
    }


    //Now lets draw our image
    //First clear the renderer
    SDL_RenderClear(render);
    //Draw the texture
    SDL_RenderCopy(render, tex, NULL, NULL);

    rectangleRGBA(render, 100, 100, 150, 150, 255, 0, 0, 255);
    roundedBoxRGBA(render, 200, 200, 300, 300, 10, 255, 0, 255, 100);

    //Update the screen
    SDL_RenderPresent(render);



}

void cleanup() {

    // SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    // Shut down SDL
    SDL_Quit();
}
