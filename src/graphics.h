#include "defs.h"

extern SDL_Window *window;
extern SDL_Renderer *render;

extern bool render_changed;

void init_graphics();
void draw_playing_field();
void setRenderChanged();
void updateRender();
