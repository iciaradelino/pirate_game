#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "player.h" // needs Player struct
#include "room.h"   // needs Room definition
#include "item.h"   // needs Item definition
#include "common.h" // for MAX_LINE_LENGTH and FILE*
#include <time.h>

#define FRIDGE_TIME_LIMIT 30 // 30 seconds time limit for the fridge

struct GameState {
    Player player;
    int game_over;
    int game_won;
    int should_restart;

    // Puzzle/Event Flags
    int cook_pleased;
    int prisoners_hostile;
    int prisoners_pacified;
    int riddle_attempts[3];
    int current_riddle_idx;
    int diary_deciphered;
    int knows_chest_code;
    int chest_unlocked;
    int captain_riddle_solved;
    int captain_riddle_attempts;

    int special_prompt_active; // If true, input goes to special handler
    char special_prompt_context[MAX_LINE_LENGTH]; // Context for special input

    // game Data
    Room rooms[NUM_ROOMS];
    Item items[MAX_ITEMS];

    FILE* log_file;

    // timer for fridge room
    time_t fridge_entry_time;
    int fridge_timer_active;
};

// function prototypes
void init_game_state(GameState* gs);

#endif // GAME_STATE_H