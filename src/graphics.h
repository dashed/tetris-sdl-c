#include "defs.h"

extern SDL_Window *window;
extern SDL_Renderer *render;
extern SDL_Texture *display;
extern TTF_Font *gFont;

// This tracks whether to update the render.
// Any procedure that has updated the render should set this to true.
extern bool render_changed;

void init_graphics();
void cleanup_graphics();


void draw_block(uint8_t x, uint8_t y, uint32_t color);

void setRenderChanged();
void preRender();
void updateRender();

