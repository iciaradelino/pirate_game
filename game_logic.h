#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game_state.h" // Needs GameState struct

// Function prototypes for command handlers and game flow
void show_room_description(GameState* gs);
void handle_move(GameState* gs, const char* direction_str);
void handle_examine(GameState* gs, const char* item_name_str);
void handle_pickup(GameState* gs, const char* item_name_str);
void handle_drop(GameState* gs, const char* item_name_str);
void handle_inventory(GameState* gs);
void handle_use(GameState* gs, const char* item_name_str);
void handle_open_chest(GameState* gs);
void process_special_input(GameState* gs, const char* raw_input);
void handle_game_over(GameState* gs, const char* message, const char* art_key);
void handle_win_game(GameState* gs);
void restart_game_flow(GameState* gs);
void handle_hint(GameState* gs);
void check_fridge_timer(GameState* gs);

#endif // GAME_LOGIC_H