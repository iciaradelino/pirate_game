#include "game_state.h"
#include "room.h" // For init_rooms()

void init_game_state(GameState *gs)
{
    int i;

    // Initialize player state
    gs->player.current_room_id      = ROOM_DECK; // Initial room (will be set after intro)
    gs->player.inventory_count      = 0;
    for (i = 0; i < MAX_INVENTORY_ITEMS; ++i)
    {
        gs->player.inventory[i]     = ITEM_NONE;
    }

    // Initialize game flags
    gs->game_over                  = 0;
    gs->game_won                   = 0;
    gs->should_restart             = 0;

    // Initialize puzzle/event flags
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

    // Initialize timer
    gs->fridge_timer_active        = 0;
    gs->fridge_entry_time          = 0;

    // Initialize special prompt state
    gs->special_prompt_active      = 0;
    gs->special_prompt_context[0]  = '\0';

    // Re-initialize rooms (clears items, resets visited status)
    // Initialize rooms *within this gs instance*
    // This is crucial for game restarts.
    init_rooms(gs); // Pass gs
}