#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For tolower
#include <time.h>   // For time() in logging

// For cross-platform sleep and clear screen
#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#define SLEEP_MS(ms) Sleep(ms)
#define PATH_MAX MAX_PATH
#else
#include <unistd.h> // For usleep
#include <limits.h> // For PATH_MAX
#define CLEAR_SCREEN() system("clear")
#define SLEEP_MS(ms) usleep(ms * 1000) // usleep takes microseconds
#endif

#define MAX_LINE_LENGTH 256
#define LOG_FILENAME "game_log.txt"
#define GUIDE_FILENAME "game_guide.txt" // Path to the game guide file
#define MAP_FILENAME "ascii/map.txt" // Path to the map file
#define SWING_ANIMATION_FILENAME "ascii/swing_animation_frames.txt" // Path for animation

// Forward declaration for GameState to avoid circular dependencies in some utils
typedef struct GameState GameState;

#endif // COMMON_H