#include "defs.h"

extern SDL_Window *window;
extern SDL_Renderer *render;

// This tracks whether to update the render.
// Any procedure that has updated the render should set this to true.
extern bool render_changed;

void init_graphics();
void cleanup_graphics();

void draw_playing_field();
void draw_block(int x, int y, uint32_t color);

void setRenderChanged();
void updateRender();
