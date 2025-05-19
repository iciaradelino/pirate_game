#include "game_state.h"
#include "room.h" // for init_rooms()

void init_game_state(GameState *gs)
{
    int i;

    // initialize player state
    gs->player.current_room_id      = ROOM_DECK; // initial room (will be set after intro)
    gs->player.inventory_count      = 0;
    for (i = 0; i < MAX_INVENTORY_ITEMS; ++i)
    {
        gs->player.inventory[i]     = ITEM_NONE;
    }

    // initialize game flags
    gs->game_over                  = 0;
    gs->game_won                   = 0;
    gs->should_restart             = 0;

    // initialize puzzle/event flags
    gs->cook_pleased               = 0;
    gs->prisoners_hostile          = 1;
    gs->prisoners_pacified         = 0;
    for (i = 0; i < 3; ++i)
    {
        gs->riddle_attempts[i]     = 3;
    }
    gs->current_riddle_idx         = 0;
    gs->diary_deciphered           = 0;
    gs->knows_chest_code           = 0;
    gs->chest_unlocked             = 0;
    gs->captain_riddle_solved      = 0; 
    gs->captain_riddle_attempts    = 3; // 3 attempts for captain's riddle

    // initialize timer
    gs->fridge_timer_active        = 0;
    gs->fridge_entry_time          = 0;

    // initialize special prompt state
    gs->special_prompt_active      = 0;
    gs->special_prompt_context[0]  = '\0';

    // re-initialize rooms (clears items, resets visited status)
    // initialize rooms *within this gs instance*
    // this is crucial for game restarts.
    init_rooms(gs); // Pass gs
}