#include "defs.h"
#include "graphics.h"

typedef struct {

    // an array of rotation schemes of a tetromino.
    // each rotation scheme is represented as 16 bits which form 4x4 matrix.
    // row-major order convention is used to interpret this matrix.
    uint16_t rotation[4];

    // RGBA convention: 0xRRGGBBAA
    uint32_t color;

} Tetromino;

const static Tetromino TETRA_Z = {
    {0xC600, 0x2640, 0x0C60, 0x4C80},
    0xFFFF00FF
};


// bool array of the playfield.
// Use row-major order convention to access (x,y) coord.
// Origin is 'top-left' -- like matrices.
// Zero-based indexing.
static bool playfield[PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH] = {false};

bool get_playfield(uint8_t x, uint8_t y);


void updateTetris();

void spawn_tetromino();
void render_tetromino(Tetromino tetra, uint8_t rotation_idx, uint8_t x, uint8_t y);

