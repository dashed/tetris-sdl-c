#include "graphics.h"

// TODO: better name?
void init_graphics() {

    render_changed = false;

    window = SDL_CreateWindow(
        // title of window
        WINDOW_TITLE,

        // initial position of the window
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,

        WINDOW_WIDTH, WINDOW_HEIGHT,
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

}

void draw_playing_field() {

    // Set rendering clear color
    // This sets the 'background color'
    SDL_SetRenderDrawColor(render, 204, 192, 179, 255);

    // Clear the render
    // 'set' background color defined in SDL_SetRenderDrawColor(...)
    SDL_RenderClear(render);

    // Draw tetris playing field
    // Vertical lines for playing field
    int i = PLAYFIELD_WIDTH * (BLOCK_SIZE + 1);
    for (; i >= 0; i -= BLOCK_SIZE + 1)
        aalineRGBA(render, i, 0, i, WINDOW_HEIGHT, 187, 173, 160, 255);

    // Horizontal lines for playing field
    i = PLAYFIELD_HEIGHT * (BLOCK_SIZE + 1);
    for (; i >= 0; i -= BLOCK_SIZE + 1)
        aalineRGBA(render, 0, i, WINDOW_WIDTH, i, 187, 173, 160, 255);


    //Update the screen
    setRenderChanged();
}


void setRenderChanged() {
    render_changed = true;
}

void updateRender() {

    // // Set rendering clear color to be black
    // SDL_SetRenderDrawColor(render, 204, 192, 179, 255);

    // // Clear the render
    // SDL_RenderClear(render);

    if(render_changed) {
        SDL_RenderPresent(render);
        render_changed = false;
    }
}

void draw_block(int x, int y, int red, int green, int blue, int alpha) {

    assert(x >= 0 && x < PLAYFIELD_WIDTH);
    assert(y >= 0 && y < PLAYFIELD_HEIGHT);

    // top-left coords of block
    int x_tl = x * (BLOCK_SIZE + 1) + 1;
    int y_tl = y * (BLOCK_SIZE + 1) + 1;

    // bottom-right coords of block
    int x_br = x_tl + BLOCK_SIZE;
    int y_br = y_tl + BLOCK_SIZE;

    boxRGBA(render, x_tl, y_tl, x_br, y_br, red, green, blue, alpha);

    setRenderChanged();

}

void cleanup_graphics() {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
}
