#include "tetris.h"


void draw_playing_field() {

    // loop var
    int i;

    // Set rendering clear color
    // This sets the 'background color'
    SDL_SetRenderDrawColor(render, 204, 192, 179, 255);

    // Clear the render
    // 'set' background color defined in SDL_SetRenderDrawColor(...)
    SDL_RenderClear(render);


    i = PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH;
    while (i --> 0)
        set_playfield(i % PLAYFIELD_WIDTH, i / PLAYFIELD_WIDTH, playfield[i]);


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

void lockTetromino() {

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

    uint8_t completed_lines = 0;

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

            completed_lines++;

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

    // update score

    if(completed_lines > 0) {
        // tetris
        score += completed_lines/4 * 800;
        completed_lines = completed_lines % 4;

        // triple
        score += completed_lines/3 * 500;
        completed_lines = completed_lines % 3;

        // double
        score += completed_lines/2 * 300;
        completed_lines = completed_lines % 2;

        // single
        score += completed_lines * 100;
    }


    spawn_tetromino();

}

void render_score() {
    // Show tetris score after all tetris operations are finished
    SDL_Color textColor = { 0x11, 0x1F, 0x3F };

    sds string_score = printfcomma(score);

    SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, string_score, textColor);

    sdsfree(string_score);

    if (textSurface == NULL) {
        fprintf(stderr,
                "\nTTF_RenderText_Solid Error:  %s\n",
                SDL_GetError());
        exit(1);
    }

    SDL_Texture* mTexture = SDL_CreateTextureFromSurface(render, textSurface);

    if (mTexture == NULL) {
        fprintf(stderr,
                "\nSDL_CreateTextureFromSurface Error:  %s\n",
                SDL_GetError());
        exit(1);
    }

    int mWidth = textSurface->w;
    int mHeight = textSurface->h;

    // render text
    SDL_Rect renderQuad = { WINDOW_WIDTH - mWidth - 10, 10, mWidth, mHeight };

    SDL_RenderCopy(render, mTexture, NULL, &renderQuad);

    SDL_DestroyTexture(mTexture);
    SDL_FreeSurface(textSurface);
}

void updateTetris() {

    if (cb_timer == 0) {
        cb_timer = SDL_AddTimer(500, auto_drop_timer, NULL);
    }

    // draw the scoreboard as needed
    int i = 4;
    bool on_score_area = false;
    while(i --> 0) {
        uint8_t x_coord = i * 2;
        uint8_t y_coord = x_coord + 1;

        // uint8_t _x = CURRENT_TETROMINO_COORDS[x_coord];
        uint8_t _y = CURRENT_TETROMINO_COORDS[y_coord];

        // if a tetromino is within the top 3 rows of the playing field, redraw
        // that area of the playing field.
        // the third row is considered when the tetromino leaves the score area,
        // it will clear the previous position.
        if(_y <= 2) {
            on_score_area = true;
            break;
        }
    }

    if(on_score_area) {

        // re-draw playfield area where score is located in
        int n = PLAYFIELD_WIDTH * 2 - 1;
        while(n --> 0) {
            int x = n % PLAYFIELD_WIDTH;
            int y = n / PLAYFIELD_WIDTH;

            set_playfield(x, y, get_playfield(x, y));
        }

        draw_playing_field();

        // re-draw tetromino
        render_current_tetromino(CURRENT_TETROMINO);

        render_score();
    }



    Tetromino_Movement request = CURRENT_TETROMINO;

    // action from keyboard
    switch(TETROMINO_ACTION) {
        case NONE:
            // do nothing - don't bother redrawing
        break;

        case ROTATE:
            request.rotation = (request.rotation + 1) % 4;
            render_current_tetromino(request);
        break;

        case LEFT:
            request.x -= 1;
            render_current_tetromino(request);
        break;

        case RIGHT:
            request.x += 1;
            render_current_tetromino(request);

        break;

        case DROP:

            request.y += 1;
            while(render_current_tetromino(request))
                request.y += 1;

            lockTetromino();

        break;

        case DOWN:
            request.y += 1;
            if(!render_current_tetromino(request)) {
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
            if (!render_current_tetromino(request)) {
                lock_delay_count++;
            } else {
                lock_delay_count = 0;
            }

            if (lock_delay_count >= lock_delay_threshold) {
                lockTetromino();
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

    if(!render_current_tetromino(tetra_request)) {

        // Reset the game
        initTetris();
    }
}

bool can_render_tetromino(Tetromino_Movement tetra_request, uint8_t block_render_queue[]) {
    uint16_t bit, piece;
    uint8_t row = 0, col = 0;

    piece = tetra_request.type.rotation[tetra_request.rotation];
    uint8_t x = tetra_request.x;
    uint8_t y = tetra_request.y;


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

                if(block_render_queue != NULL) {
                    block_render_queue[i * 2] = _x;
                    block_render_queue[i * 2 + 1] = _y;
                }

                i++;
            }
        }

        // cycle col between 0 to 3
        // if col is 0 then increment row
        col++;
        col = col % 4;
        if(col == 0) {
            row++;
        }

    }

    return true;

}

bool render_current_tetromino(Tetromino_Movement tetra_request) {

    // create ghost
    Tetromino ghost = tetra_request.type;

    // change alpha to ~50%
    ghost.color = ghost.color & 0x00FFFFFF;
    ghost.color = ghost.color | 0x66000000;

    Tetromino_Movement ghost_request = tetra_request;
    ghost_request.type = ghost;

    // render ghost tetromino
    while(render_tetromino(ghost_request, GHOST_TETROMINO_COORDS))
        ghost_request.y += 1;

    // change alpha to 90%
    tetra_request.type.color = tetra_request.type.color & 0x00FFFFFF;
    tetra_request.type.color = tetra_request.type.color | 0xE5000000;

    if(render_tetromino(tetra_request, CURRENT_TETROMINO_COORDS)) {
        CURRENT_TETROMINO = tetra_request;

        return true;
    }

    return false;
}

// render tetromino movement request
// returns true if tetromino is rendered succesfully; false otherwise
bool render_tetromino(Tetromino_Movement tetra_request, uint8_t current_coords[]) {

    // simple 'queue' to store coords of blocks to render on playing field.
    // Each tetromino has 4 blocks with total of 4 coordinates.
    //
    // To access a coord, if 0 <= i < 4, then
    //      x = i * 2, y = x + 1
    //
    uint8_t block_render_queue[8] = {0};

    if(!can_render_tetromino(tetra_request, block_render_queue))
        return false;

    // clear old tetromino position
    int i = 4;
    while(i --> 0) {
        uint8_t x_coord = i * 2;
        uint8_t y_coord = x_coord + 1;

        uint8_t _x = current_coords[x_coord];
        uint8_t _y = current_coords[y_coord];

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

        current_coords[x_coord] = _x;
        current_coords[y_coord] = _y;

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
