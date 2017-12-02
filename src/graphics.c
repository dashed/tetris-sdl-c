#include "graphics.h"


void init_graphics() {

    render_changed = false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    window = SDL_CreateWindow(
        // title of window
        WINDOW_TITLE,

        // initial position of the window
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,

        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

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
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    if (render == NULL) {
        fprintf(stderr,
                "\nSDL_CreateRenderer Error:  %s\n",
                SDL_GetError());
        exit(1);
    }

    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

    // texture for render context
    display = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_SetRenderTarget(render, display);

    // Load font
    gFont = TTF_OpenFont("src/font/Inconsolata-Regular.ttf", 30);

    if (gFont == NULL) {
        fprintf(stderr,
                "\nTTF_OpenFont Error:  %s\n",
                SDL_GetError());
        exit(1);
    }

    TTF_SetFontHinting(gFont, TTF_HINTING_MONO);
}

void setRenderChanged() {
    render_changed = true;
}

void preRender() {


    SDL_SetRenderTarget(render, display);


}

void updateRender() {

    // lazily update the screen only if render operations are queued
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

    // top-right coords of block
    uint16_t x_tr = x_tl + BLOCK_SIZE;
    uint16_t y_tr = y_tl;

    // bottom-right coords of block
    uint16_t x_br = x_tl + BLOCK_SIZE;
    uint16_t y_br = y_tl + BLOCK_SIZE;

    // bottom-left coords of block
    uint16_t x_bl = x_tl;
    uint16_t y_bl = y_tl + BLOCK_SIZE;

    boxColor(render, x_tl, y_tl, x_br, y_br, color);

    // draw grid lines

    if(y <= 0) {
        // draw top horizontal grid line (from top-left to top-right)
        aalineRGBA(render, x_tl, y_tl, x_tr, y_tr, 187, 173, 160, 255);
    }

    if(x <= 0) {
        // draw left vertical grid line (from top-left to bottom-left)
        aalineRGBA(render, x_tl, y_tl, x_bl, y_bl, 187, 173, 160, 255);
    }

    // draw bottom horizontal grid line (from bottom-left to bottom-right)
    aalineRGBA(render, x_bl, y_bl, x_br, y_br, 187, 173, 160, 255);

    // draw right vertical grid line (from top-right to bottom-right)
    aalineRGBA(render, x_tr, y_tr, x_br, y_br, 187, 173, 160, 255);

    setRenderChanged();

}

void cleanup_graphics() {
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
}
