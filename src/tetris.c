#include "tetris.h"

bool lol = false;

void updateTetris() {

    if(!lol) {
        render_tetromino(TETRA_Z, 0, 3, 0);
        lol = true;
    }


}

void spawn_tetromino() {

}

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
                break;
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

    // render tetromino blocks
    i = 4;
    while(i --> 0) {
        int _x = block_render_queue[i * 2];
        int _y = block_render_queue[i * 2 + 1];

        draw_block(_x, _y, tetra.color);
    }

}

bool get_playfield(uint8_t x, uint8_t y) {
    return playfield[(y * PLAYFIELD_WIDTH) + x];
}
