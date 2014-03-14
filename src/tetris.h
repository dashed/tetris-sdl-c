#include "defs.h"
#include "utility.h"
#include "shuffle.h"
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

typedef struct {

    Tetromino type;

    // expected values from 0 to 4 which are the indices of Tetromino.rotation
    uint8_t rotation;

    uint8_t x;
    uint8_t y;

} Tetromino_Movement;

typedef enum {
    NONE,
    DOWN,
    LEFT,
    RIGHT,
    DROP,
    ROTATE,
    // soft-drop tetrominos
    AUTO_DROP,
    RESTART
} Tetris_Action;

typedef enum {
    EMPTY = 0xFFB3C0CC,
    TEAL = 0xFFFFDB7F,
    BLUE = 0xFFD97400,
    ORANGE = 0XFF1B85FF,
    YELLOW = 0xFF00DCFF,
    GREEN = 0xFF40CC2E,
    PURPLE = 0xFF4B1485,
    RED = 0xFF4B59F2

} Color_Block;

// default tetris action
// defines the action to apply to current tetromino
extern Tetris_Action TETROMINO_ACTION;

// tetromino data
const static Tetromino TETRA_I = {
    {0x0F00, 0x2222, 0x00F0, 0x4444},
    TEAL
};

const static Tetromino TETRA_J = {
    {0x8E00, 0x6440, 0x0E20, 0x44C0},
    BLUE
};

const static Tetromino TETRA_L = {
    {0x2E00, 0x4460, 0x0E80, 0xC440},
    ORANGE
};

const static Tetromino TETRA_O = {
    {0x6600, 0x6600, 0x6600, 0x6600},
    YELLOW
};

const static Tetromino TETRA_S = {
    {0x6C00, 0x4620, 0x06C0, 0x8c40},
    GREEN
};

const static Tetromino TETRA_T = {
    {0x4E00, 0x4640, 0x0E40, 0x4C40},
    PURPLE
};

const static Tetromino TETRA_Z = {
    {0xC600, 0x2640, 0x0C60, 0x4C80},
    RED
};

// simple array to store coords of blocks rendered on playing field.
// Each tetromino has 4 blocks with total of 4 coordinates.
//
// To access a coord, if 0 <= i < 4, then
//      x = i * 2, y = x + 1
//
static uint8_t CURRENT_TETROMINO_COORDS[8] = {0};
static uint8_t GHOST_TETROMINO_COORDS[8] = {0};

static Tetromino_Movement CURRENT_TETROMINO;


// bool array of the playfield.
// Use row-major order convention to access (x,y) coord.
// Origin is 'top-left' -- like matrices.
// Zero-based indexing.
static Color_Block playfield[PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH];


// Every time AUTO_DROP event is executed, the current tetromino will drop by one
// block. If the drop is unsucessful equal to the number of times of lock_delay_threshold,
// the tetromino freezes in place.
//
// Lock when ++lock_delay_count % lock_delay_threshold == 0
const static uint8_t lock_delay_threshold = 2;
static uint8_t lock_delay_count = 0;

// Queue to determine the next tetromino.
// Knuth shuffle algorithm is applied.
static uint8_t tetromino_queue[7 * 4];
static uint8_t tetromino_queue_size = 7*4;
static uint8_t current_queue_index = 0;


static SDL_TimerID cb_timer = 0;

static int score = 0;

#endif


void draw_playing_field();
Color_Block get_playfield(uint8_t x, uint8_t y);
void set_playfield(uint8_t x, uint8_t y, Color_Block color);

void initTetris();
void updateTetris();
void lockTetromino();

void spawn_tetromino();
bool render_tetromino(Tetromino_Movement tetra_request, uint8_t current_coords[]);
bool render_current_tetromino(Tetromino_Movement tetra_request);
