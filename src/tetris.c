#include "tetris.h"

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


    // Update the screen
    setRenderChanged();
}

// TODO: remove
bool lol = false;
void updateTetris() {

    if(!lol) {
        spawn_tetromino();
        lol = true;
    }

    switch(TETROMINO_ACTION) {
        case NONE:
            render_tetromino(CURRENT_TETROMINO_TYPE, CURRENT_ROT, CURRENT_POS[0], CURRENT_POS[1]);
        break;

        case ROTATE:
        break;

        case LEFT:
        break;

        case RIGHT:
        break;

        case DROP:
        break;

        case DOWN:
            render_tetromino(CURRENT_TETROMINO_TYPE, CURRENT_ROT, CURRENT_POS[0], CURRENT_POS[1] + 1);
            TETROMINO_ACTION = NONE;
        break;
    }


}

void spawn_tetromino() {
    render_tetromino(TETRA_Z, 0, 3, 11);
}

// render current tetromino
void render_tetromino(Tetromino tetra, uint8_t rotation_idx, uint8_t x, uint8_t y) {

    uint16_t bit, piece;
    uint8_t row = 0, col = 0;

    piece = tetra.rotation[rotation_idx];

    // simple 'queue' to store coords of blocks to render on playing field.
    // Each tetromino has 4 blocks with total of 4 coordinates.
    //
    // To access a coord, if 0 <= i < 4, then
    //      x = i * 2, y = x + 1
    //
    uint8_t block_render_queue[8] = {0};
    bool render_tetromino_block = true;

    // loop through tetramino data
    int i = 0;
    for (bit = 0x8000; bit > 0 && i < 8; bit = bit >> 1) {

        if (piece & bit) {

            uint8_t _x = x + col;
            uint8_t _y = y + row;

            // bounds check
            if ((_x < 0) || (_x >= PLAYFIELD_WIDTH)
                || (_y < 0) || (_y >= PLAYFIELD_HEIGHT)
                || get_playfield(_x, _y) == true) {

                // unable to render tetramino block
                render_tetromino_block = false;

                printf("UNABLE TO MOVE (%d, %d)\n", _x, _y);

                break;
                return;
            } else {

                block_render_queue[i * 2] = _x;
                block_render_queue[i * 2 + 1] = _y;
                i++;
            }

            // draw_block(_x, _y, 0, 255, 0, 255);
        }

        // cycle col between 0 to 3
        // if col is 0 then increment row
        ((col = ++col % 4) == 0 && ++row);

    }

    if(!render_tetromino_block) {
        return;
    }

    // TODO: put all of this into a struct
    CURRENT_TETROMINO_TYPE = tetra;
    CURRENT_ROT = rotation_idx;

    // update new position
    CURRENT_POS[0] = x;
    CURRENT_POS[1] = y;

    // clear old tetromino position
    i = 4;
    while(i --> 0) {
        uint8_t x_coord = i * 2;
        uint8_t y_coord = x_coord + 1;


        uint8_t _x = CURRENT_TETROMINO_COORDS[x_coord];
        uint8_t _y = CURRENT_TETROMINO_COORDS[y_coord];

        // printf("clear (%d, %d)\n", _x, _y);

        draw_block(_x, _y, DEFAULT_BLOCK_COLOR);
    }

    // printf("...\n");

    // render tetromino blocks
    i = 4;
    while(i --> 0) {

        uint8_t x_coord = i * 2;
        uint8_t y_coord = x_coord + 1;


        // uint8_t _x = CURRENT_TETROMINO_COORDS[x_coord];
        // uint8_t _y = CURRENT_TETROMINO_COORDS[y_coord];

        // draw_block(_x, _y, DEFAULT_BLOCK_COLOR);

        // store and draw new tetromino position
        uint8_t _x = block_render_queue[x_coord];
        uint8_t _y = block_render_queue[y_coord];

        CURRENT_TETROMINO_COORDS[x_coord] = _x;
        CURRENT_TETROMINO_COORDS[y_coord] = _y;

        draw_block(_x, _y, tetra.color);

        // printf("draw (%d, %d)\n", _x, _y);
    }

    // printf("...\n");

}

bool get_playfield(uint8_t x, uint8_t y) {
    return playfield[(y * PLAYFIELD_WIDTH) + x];
}
