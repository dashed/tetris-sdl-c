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

    display = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_SetRenderTarget(render, display);
}




void setRenderChanged() {
    render_changed = true;
}

void preRender() {
    // Set rendering clear color
    // This sets the 'background color'
    // SDL_SetRenderDrawColor(render, 204, 192, 179, 255);

    // Clear the render
    // 'set' background color defined in SDL_SetRenderDrawColor(...)
    // SDL_RenderClear(render);

    SDL_SetRenderTarget(render, display);
}

void updateRender() {

    // // Set rendering clear color to be black
    // SDL_SetRenderDrawColor(render, 204, 192, 179, 255);

    // // Clear the render
    // SDL_RenderClear(render);

    if(render_changed) {

        SDL_SetRenderTarget(render, NULL);
        SDL_RenderCopy(render, display, NULL, NULL);

        SDL_RenderPresent(render);
        render_changed = false;


    }
}

void draw_block(uint8_t x, uint8_t y, uint32_t color) {

    assert(x >= 0 && x < PLAYFIELD_WIDTH);
    assert(y >= 0 && y < PLAYFIELD_HEIGHT);

    // top-left coords of block
    uint16_t x_tl = x * (BLOCK_SIZE + 1) + 1;
    uint16_t y_tl = y * (BLOCK_SIZE + 1) + 1;

    // bottom-right coords of block
    uint16_t x_br = x_tl + BLOCK_SIZE;
    uint16_t y_br = y_tl + BLOCK_SIZE;

    boxColor(render, x_tl, y_tl, x_br, y_br, color);

    setRenderChanged();

}

void cleanup_graphics() {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
}
