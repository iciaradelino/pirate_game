#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h" // for GameState forward dec, FILE*, etc.

#define FRAME_DELIMITER "---FRAME_END---"
#define MAX_FRAME_LINES 50  // max number of lines a frame can have

int run_animation(const char* filename, GameState* gs);

#endif // ANIMATION_H