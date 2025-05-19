#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN()    system("cls")
    #define SLEEP_MS(ms)     Sleep(ms)
    #define PATH_MAX         MAX_PATH
#else
    #include <unistd.h>
    #include <limits.h>
    #define CLEAR_SCREEN()    system("clear")
    #define SLEEP_MS(ms)     usleep((ms) * 1000)
#endif

#define MAX_LINE_LENGTH              256
#define LOG_FILENAME                 "game_log.txt"
#define GUIDE_FILENAME               "game_guide.txt"
#define MAP_FILENAME                 "ascii/map.txt"
#define SWING_ANIMATION_FILENAME     "ascii/swing_animation_frames.txt"

typedef struct GameState GameState;

#endif // COMMON_H