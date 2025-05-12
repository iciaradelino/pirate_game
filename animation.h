#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h" // For GameState forward dec, FILE*, etc.

#define ANIMATION_FILENAME "./ascii/swing_animation_frames.txt"
#define FRAME_DELIMITER "---FRAME_END---"
#define MAX_FRAME_LINES 50  // Max number of lines a frame can have

// Function prototype
int run_animation(const char* filename, GameState* gs);

#endif // ANIMATION_H