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

Uint32 auto_drop_timer(Uint32 interval, void *param) {

    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return interval;
}

void initTetris() {

    // set up SDL timer
    if(cb_timer != 0) {
        SDL_RemoveTimer(cb_timer);
    }
    cb_timer = 0;

    TETROMINO_ACTION = NONE;

    // Empty the playfield
    int i = PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH;
    while (i --> 0) {
        playfield[i] = EMPTY;
    }

    // build tetromino queue
    current_queue_index = 0;
    i = tetromino_queue_size;
    int n = 0;
    while(i --> 0) {
        if((i + 1) % 4 == 0) {
            n++;
        }
        tetromino_queue[i] = n;
    }

    // apply shuffle algorithm
    shuffle(tetromino_queue, tetromino_queue_size, sizeof(uint8_t));

    draw_playing_field();

    spawn_tetromino();

}

void updateTetris() {

    if (cb_timer == 0) {
        cb_timer = SDL_AddTimer(500, auto_drop_timer, NULL);
    }

    Tetromino_Movement request = CURRENT_TETROMINO;

    // action from keyboard
    switch(TETROMINO_ACTION) {
        case NONE:
            // do nothing
        break;

        case ROTATE:
            request.rotation = (request.rotation + 1) % 4;
            render_tetromino(request);
        break;

        case LEFT:
            request.x -= 1;
            render_tetromino(request);
        break;

        case RIGHT:
            request.x += 1;
            render_tetromino(request);

        break;

        case DROP:

        break;

        case DOWN:
            request.y += 1;
            if(!render_tetromino(request)) {
                lock_delay_count++;
            } else {
                lock_delay_count = 0;
            }
        break;

        case RESTART:
            initTetris();
        break;

        case AUTO_DROP:

            request.y += 1;
            if (!render_tetromino(request)) {
                lock_delay_count++;
            } else {
                lock_delay_count = 0;
            }

            if (lock_delay_count >= lock_delay_threshold) {
                lock_delay_count = 0;

                // lock tetromino in place
                int i = 4;
                while(i --> 0) {
                    uint8_t x_coord = i * 2;
                    uint8_t y_coord = x_coord + 1;

                    uint8_t _x = CURRENT_TETROMINO_COORDS[x_coord];
                    uint8_t _y = CURRENT_TETROMINO_COORDS[y_coord];

                    CURRENT_TETROMINO_COORDS[x_coord] = 0;
                    CURRENT_TETROMINO_COORDS[y_coord] = 0;

                    set_playfield(_x, _y, CURRENT_TETROMINO.type.color);
                }

                // clear lines if any
                uint8_t row = PLAYFIELD_HEIGHT;
                int8_t row_to_copy_to = -1;
                while(row --> 0) {
                    uint8_t col;
                    bool complete_line = true;

                    // check if line is complete
                    for(col = 0; col < PLAYFIELD_WIDTH; col++) {
                        if(get_playfield(col, row) == EMPTY) {

                            complete_line = false;
                            break;
                        }
                    }

                    // clear line
                    if(complete_line) {

                        if(row_to_copy_to < row) {
                            row_to_copy_to = row;
                        }

                        for(col = 0; col < PLAYFIELD_WIDTH; col++) {
                            set_playfield(col, row, EMPTY);
                        }

                    } else if(row_to_copy_to > row) {

                        for(col = 0; col < PLAYFIELD_WIDTH; col++) {
                            set_playfield(col, row_to_copy_to, get_playfield(col, row));
                        }

                        row_to_copy_to--;
                    }

                }

                spawn_tetromino();
            }

        break;
    }
    TETROMINO_ACTION = NONE;

}

void spawn_tetromino() {

    current_queue_index++;
    if(current_queue_index >= tetromino_queue_size) {
        current_queue_index = 0;

        // apply shuffle algorithm
        shuffle(tetromino_queue, tetromino_queue_size, sizeof(uint8_t));
    }

    Tetromino type;

    switch(tetromino_queue[current_queue_index]) {
        case 1:
            type = TETRA_I;
        break;
        case 2:
            type = TETRA_J;
        break;
        case 3:
            type = TETRA_L;
        break;
        case 4:
            type = TETRA_O;
        break;
        case 5:
            type = TETRA_S;
        break;
        case 6:
            type = TETRA_T;
        break;
        case 7:
            type = TETRA_Z;
        break;
    }

    Tetromino_Movement tetra_request = {
        type,
        0,
        3, 0
    };

    if(!render_tetromino(tetra_request)) {

        // Reset the game
        initTetris();
    }
}

// render tetromino movement request
// returns true if tetromino is rendered succesfully; false otherwise
bool render_tetromino(Tetromino_Movement tetra_request) {

    uint16_t bit, piece;
    uint8_t row = 0, col = 0;

    piece = tetra_request.type.rotation[tetra_request.rotation];
    uint8_t x = tetra_request.x;
    uint8_t y = tetra_request.y;

    // simple 'queue' to store coords of blocks to render on playing field.
    // Each tetromino has 4 blocks with total of 4 coordinates.
    //
    // To access a coord, if 0 <= i < 4, then
    //      x = i * 2, y = x + 1
    //
    uint8_t block_render_queue[8] = {0};

    // loop through tetramino data
    int i = 0;
    for (bit = 0x8000; bit > 0 && i < 8; bit = bit >> 1) {

        if (piece & bit) {

            uint8_t _x = x + col;
            uint8_t _y = y + row;

            // bounds check
            if ((_x < 0) || (_x >= PLAYFIELD_WIDTH)
                || (_y < 0) || (_y >= PLAYFIELD_HEIGHT)
                || get_playfield(_x, _y) != EMPTY) {

                // unable to render tetramino block
                return false;
                break;
            } else {

                block_render_queue[i * 2] = _x;
                block_render_queue[i * 2 + 1] = _y;
                i++;
            }
        }

        // cycle col between 0 to 3
        // if col is 0 then increment row
        ((col = ++col % 4) == 0 && ++row);

    }

    // save tetromino as the new current
    CURRENT_TETROMINO = tetra_request;

    // clear old tetromino position
    i = 4;
    while(i --> 0) {
        uint8_t x_coord = i * 2;
        uint8_t y_coord = x_coord + 1;

        uint8_t _x = CURRENT_TETROMINO_COORDS[x_coord];
        uint8_t _y = CURRENT_TETROMINO_COORDS[y_coord];

        draw_block(_x, _y, EMPTY);
    }

    // render new tetromino blocks
    i = 4;
    while(i --> 0) {

        uint8_t x_coord = i * 2;
        uint8_t y_coord = x_coord + 1;

        // store and draw new tetromino position
        uint8_t _x = block_render_queue[x_coord];
        uint8_t _y = block_render_queue[y_coord];

        CURRENT_TETROMINO_COORDS[x_coord] = _x;
        CURRENT_TETROMINO_COORDS[y_coord] = _y;

        draw_block(_x, _y, tetra_request.type.color);

    }

    return true;
}

Color_Block get_playfield(uint8_t x, uint8_t y) {
    return playfield[(y * PLAYFIELD_WIDTH) + x];
}

void set_playfield(uint8_t x, uint8_t y, Color_Block color) {
    playfield[(y * PLAYFIELD_WIDTH) + x] = color;

    draw_block(x, y, color);
}
