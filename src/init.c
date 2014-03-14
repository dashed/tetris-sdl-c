#include "init.h"


void init() {

    // //SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
    // //this lets us choose when to upload or remove textures from the GPU
    // SDL_Surface *bmp = SDL_LoadBMP("src/res/hello.bmp");
    // if (bmp == NULL){

    //     fprintf(stderr,
    //             "\nSDL_LoadBMP Error:  %s\n",
    //             SDL_GetError());
    //     exit(1);
    // }

    // //To use a hardware accelerated texture for rendering we can create one from
    // //the surface we loaded
    // SDL_Texture *tex = SDL_CreateTextureFromSurface(render, bmp);
    // //We no longer need the surface
    // SDL_FreeSurface(bmp);
    // if (tex == NULL){

    //     fprintf(stderr,
    //             "\nSDL_CreateTextureFromSurface Error:  %s\n",
    //             SDL_GetError());
    //     exit(1);
    // }

    init_graphics();

    draw_playing_field();

}

void cleanup() {

    cleanup_graphics();

    // Shut down SDL
    SDL_Quit();
}
