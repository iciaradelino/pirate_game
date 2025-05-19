#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h"

#define FRAME_DELIMITER   "---FRAME_END---"
#define MAX_FRAME_LINES   50

int run_animation(const char *filename, GameState *gs);

#endif // ANIMATION_H