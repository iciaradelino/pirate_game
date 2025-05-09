#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "player.h" // Needs Player struct
#include "common.h" // For MAX_LINE_LENGTH and FILE*

struct GameState {
    Player player;
    int game_over;
    int game_won;
    int should_restart; // Flag to indicate if a restart is pending

    // Puzzle/Event Flags
    int cook_pleased;
    int prisoners_hostile;
    int prisoners_pacified;
    int riddle_attempts[3];
    int current_riddle_idx;
    int diary_deciphered;
    int knows_chest_code;
    int chest_unlocked;

    int special_prompt_active; // If true, input goes to special handler
    char special_prompt_context[MAX_LINE_LENGTH]; // Context for special input

    FILE* log_file;
};

// Function prototypes
void init_game_state(GameState* gs);

#endif // GAME_STATE_H