#include "input.h"

void getInput() {
    SDL_Event event;

    /* Loop through waiting messages and process them */

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            /* Closing the Window or pressing Escape will exit the program */
            case SDL_QUIT:
                exit(0);
            break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        exit(0);
                    break;

                    case SDLK_s:
                        TETROMINO_ACTION = DOWN;
                    break;

                    case SDLK_d:
                        TETROMINO_ACTION = RIGHT;
                    break;

                    case SDLK_a:
                        TETROMINO_ACTION = LEFT;
                    break;

                    case SDLK_w:
                        TETROMINO_ACTION = ROTATE;
                    break;

                    default:
                    break;
                }
            break;

            case SDL_KEYUP:
                TETROMINO_ACTION = NONE;
            break;

            default:
            break;
        }
    }
}
