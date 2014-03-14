#include "defs.h"
#include "graphics.h"


#ifndef _TETRIS_CONSTANTS
#define _TETRIS_CONSTANTS

typedef struct {

    // an array of rotation schemes of a tetromino.
    // each rotation scheme is represented as 16 bits which form 4x4 matrix.
    // row-major order convention is used to interpret this matrix.
    uint16_t rotation[4];

    // RGBA convention: 0xAABBGGRR
    uint32_t color;

} Tetromino;

typedef enum {
    NONE,
    DOWN,
    LEFT,
    RIGHT,
    DROP,
    ROTATE
} Tetris_Action;

// default tetris action
// defines the action to apply to current tetromino
extern Tetris_Action TETROMINO_ACTION;


// default color of an unfilled block
const static uint32_t DEFAULT_BLOCK_COLOR = 0xFFB3C0CC;
const static uint32_t BLOCK_RED = 0xFF4136FF;

// tetromino data
const static Tetromino TETRA_Z = {
    {0xC600, 0x2640, 0x0C60, 0x4C80},
    BLOCK_RED
};

// simple array to store coords of blocks rendered on playing field.
// Each tetromino has 4 blocks with total of 4 coordinates.
//
// To access a coord, if 0 <= i < 4, then
//      x = i * 2, y = x + 1
//
// TODO: put all of this into a struct
static uint8_t CURRENT_TETROMINO_COORDS[8] = {0};
static uint8_t CURRENT_POS[2] = {0}; // x, y
static uint8_t CURRENT_ROT = 0;
static Tetromino CURRENT_TETROMINO_TYPE;

// bool array of the playfield.
// Use row-major order convention to access (x,y) coord.
// Origin is 'top-left' -- like matrices.
// Zero-based indexing.
static bool playfield[PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH] = {false};

#endif




void draw_playing_field();
bool get_playfield(uint8_t x, uint8_t y);


void updateTetris();

void spawn_tetromino();
void render_tetromino(Tetromino tetra, uint8_t rotation_idx, uint8_t x, uint8_t y);

