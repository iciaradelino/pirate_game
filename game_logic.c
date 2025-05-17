#include "game_logic.h"
#include "item.h"
#include "room.h"
#include "player.h"
#include "utils.h"
#include "animation.h" // For run_animation in restart_game_flow
#include "game_state.h"
#include "parser.h"

#include <stdio.h> // For sprintf
#include <string.h>
#include <strings.h> // For strcasecmp on MinGW
#include <stdlib.h> // For rand, srand
#include <time.h>   // For time

// --- Room Description ---
void show_room_description(GameState* gs) {
    Room* current_room = &gs->rooms[gs->player.current_room_id];
    char buffer[MAX_LINE_LENGTH * 2]; // Increased buffer size

    sprintf(buffer, "\n--- %s ---", current_room->name);
    log_action(gs, "INFO", buffer);

    if (!current_room->visited || gs->player.current_room_id == ROOM_DECK) { // Always full desc for deck on new entry
        log_action(gs, "INFO", current_room->description);
        current_room->visited = 1;
    } else {
        sprintf(buffer, "You are back in the %s.", current_room->name);
        log_action(gs, "INFO", buffer);
    }

    // List items in room
    int items_listed = 0;
    strcpy(buffer, "You see: ");
    for (int i = 0; i < current_room->item_count; ++i) {
        Item* item = &gs->items[current_room->items_in_room[i]];
        if (item->id != ITEM_NONE) {
            if (items_listed > 0) strcat(buffer, ", ");
            strcat(buffer, "["); // Add brackets for clarity
            strcat(buffer, item->name);
            strcat(buffer, "]");
            items_listed++;
        }
    }
    if (items_listed > 0) {
        log_action(gs, "INFO", buffer);
    } else {
        log_action(gs, "INFO", "The room is empty of notable items.");
    }

    // List exits
    strcpy(buffer, "Exits: ");
    int exits_listed = 0;
    if (current_room->exits[0] != NUM_ROOMS) { strcat(buffer, "North "); exits_listed++; }
    if (current_room->exits[1] != NUM_ROOMS) { strcat(buffer, "South "); exits_listed++; }
    if (current_room->exits[2] != NUM_ROOMS) { strcat(buffer, "East ");  exits_listed++; }
    if (current_room->exits[3] != NUM_ROOMS) { strcat(buffer, "West ");  exits_listed++; }
    if (exits_listed > 0) {
        log_action(gs, "INFO", buffer);
    } else {
        log_action(gs, "INFO", "There are no obvious exits.");
    }
}

// --- Game Over / Win ---
void handle_game_over(GameState* gs, const char* message, const char* art_key) {
    log_action(gs, "GAME_EVENT", message);
    if (art_key) display_ascii_art(art_key);
    gs->game_over = 1;
    gs->should_restart = 1;
}

void handle_win_game(GameState* gs) {
    log_action(gs, "GAME_EVENT", "YOU HAVE FOUND THE TREASURE!");
    display_ascii_art("WIN_GAME");
    gs->game_won = 1;
}

// --- Movement ---
void handle_move(GameState* gs, const char* direction_str) {
    Room* current_room = &gs->rooms[gs->player.current_room_id];
    RoomID next_room_id = NUM_ROOMS;
    int dir_idx = -1;

    if (strcmp(direction_str, "north") == 0) dir_idx = 0;
    else if (strcmp(direction_str, "south") == 0) dir_idx = 1;
    else if (strcmp(direction_str, "east") == 0) dir_idx = 2;
    else if (strcmp(direction_str, "west") == 0) dir_idx = 3;

    if (dir_idx != -1) {
        next_room_id = current_room->exits[dir_idx];
    }

    if (next_room_id != NUM_ROOMS) {
        // Pre-entry checks
        if (next_room_id == ROOM_CAPTAIN_QUARTERS && gs->player.current_room_id == ROOM_GALLEY) {
            if (!gs->cook_pleased) { // Cook is NOT pleased
                log_action(gs, "GAME_INFO", "The Cook blocks your path north. 'Not until ye fetch me ingredients, matey! I need some [Salted Pork], [Hardtack Biscuits], and a [Grog Bottle] for the Captain\'s stew.'");
                return;
            } else { // Cook IS pleased, and player is moving North
                log_action(gs, "GAME_INFO", "The Cook nods as you pass. 'The stew was fine. Go on about yer business.'");
                // No return here, allow movement
            }
        }
        if (next_room_id == ROOM_CAPTAIN_QUARTERS && player_has_item(&gs->player, ITEM_PARROT)) {
            handle_game_over(gs, "The parrot suddenly squawks loudly, 'Intruder! Intruder!' The sleeping Captain jolts awake and shoots you!", "GAME_OVER_CAPTAIN");
            return;
        }
        if (next_room_id == ROOM_TREASURE_ROOM && !player_has_item(&gs->player, ITEM_TREASURE_KEY)) {
            log_action(gs, "GAME_INFO", "The massive door to the treasure room is sealed with an intricate lock. It won't budge.");
            return;
        }
        if (next_room_id == ROOM_TREASURE_ROOM && player_has_item(&gs->player, ITEM_TREASURE_KEY)) {
            log_action(gs, "GAME_INFO", "You use the Treasure Key. With a satisfying thunk, the heavy door unlatches!");
        }

        gs->player.current_room_id = next_room_id;
        // gs->rooms[next_room_id].visited = 0; // Reset visited if you want full description always on entry
        show_room_description(gs);

        // Post-entry events
        if (next_room_id == ROOM_TREASURE_ROOM) {
            handle_win_game(gs);
        } else if (next_room_id == ROOM_PRISON_HOLD && gs->prisoners_hostile && !gs->prisoners_pacified) {
            log_action(gs, "GAME_EVENT", "You open the door to a dark, damp hold. Three shadowy Prisoners lunge at you with makeshift shivs!");
            log_action(gs, "PROMPT", "The prisoners are about to attack! What do you do? > ");
            gs->special_prompt_active = 1;
            strcpy(gs->special_prompt_context, "PRISONER_ATTACK");
        }
    } else {
        log_action(gs, "GAME_ERROR", "You can't go that way.");
    }
}

// --- Item Interaction ---
void handle_examine(GameState* gs, const char* item_name_str) {
    if (item_name_str == NULL) {
        show_room_description(gs);
        return;
    }

    Item* item_to_examine = NULL;
    Room* current_room = &gs->rooms[gs->player.current_room_id];

    // Check room items first
    for (int i = 0; i < current_room->item_count; ++i) {
        ItemID current_item_id = current_room->items_in_room[i];
        if (strcasecmp(gs->items[current_item_id].name, item_name_str) == 0) {
            item_to_examine = &gs->items[current_item_id];
            break;
        }
    }
    // If not in room, check inventory
    if (!item_to_examine) {
        for (int i = 0; i < gs->player.inventory_count; ++i) {
            ItemID current_item_id = gs->player.inventory[i];
            if (strcasecmp(gs->items[current_item_id].name, item_name_str) == 0) {
                item_to_examine = &gs->items[current_item_id];
                break;
            }
        }
    }

    if (item_to_examine) {
        log_action(gs, "EXAMINE", item_to_examine->examine_text);
    } else {
        char buffer[MAX_LINE_LENGTH];
        sprintf(buffer, "You don't see any '%s' here to examine.", item_name_str);
        log_action(gs, "GAME_ERROR", buffer);
    }
}

void handle_pickup(GameState* gs, const char* item_name_str) {
    if (!item_name_str) {
        log_action(gs, "GAME_ERROR", "Pick up what?");
        return;
    }

    Room* current_room = &gs->rooms[gs->player.current_room_id];
    ItemID item_id_to_pickup = ITEM_NONE;

    for (int i = 0; i < current_room->item_count; ++i) {
        if (strcasecmp(gs->items[current_room->items_in_room[i]].name, item_name_str) == 0) {
            item_id_to_pickup = current_room->items_in_room[i];
            break;
        }
    }

    if (item_id_to_pickup != ITEM_NONE) {
        Item* item_ptr = &gs->items[item_id_to_pickup];
        if (item_ptr->can_pickup) {
            if (gs->player.inventory_count < MAX_INVENTORY_ITEMS) {
                add_item_to_inventory(&gs->player, item_id_to_pickup);
                remove_item_from_room(current_room, item_id_to_pickup);
                char buffer[MAX_LINE_LENGTH];
                sprintf(buffer, "You picked up the %s.", item_ptr->name);
                log_action(gs, "ACTION", buffer);
            } else {
                log_action(gs, "GAME_ERROR", "Your inventory is full.");
            }
        } else {
            char buffer[MAX_LINE_LENGTH];
            sprintf(buffer, "You can't pick up the %s.", item_ptr->name);
            log_action(gs, "GAME_ERROR", buffer);
        }
    } else {
        char buffer[MAX_LINE_LENGTH];
        sprintf(buffer, "You don't see any '%s' here to pick up.", item_name_str);
        log_action(gs, "GAME_ERROR", buffer);
    }
}

void handle_drop(GameState* gs, const char* item_name_str) {
    if (!item_name_str) {
        log_action(gs, "GAME_ERROR", "Drop what?");
        return;
    }
    ItemID item_id_to_drop = ITEM_NONE;

    for (int i = 0; i < gs->player.inventory_count; ++i) {
        if (strcasecmp(gs->items[gs->player.inventory[i]].name, item_name_str) == 0) {
            item_id_to_drop = gs->player.inventory[i];
            break;
        }
    }

    if (item_id_to_drop != ITEM_NONE) {
        Room* current_room = &gs->rooms[gs->player.current_room_id];
        if (current_room->item_count < MAX_ROOM_ITEMS) {
            add_item_to_room(current_room, item_id_to_drop);
            remove_item_from_inventory(&gs->player, item_id_to_drop);
            char buffer[MAX_LINE_LENGTH];
            sprintf(buffer, "You dropped the %s.", gs->items[item_id_to_drop].name);
            log_action(gs, "ACTION", buffer);
        } else {
            log_action(gs, "GAME_ERROR", "There's no space to drop that here.");
        }
    } else {
        char buffer[MAX_LINE_LENGTH];
        sprintf(buffer, "You don't have any '%s' to drop.", item_name_str);
        log_action(gs, "GAME_ERROR", buffer);
    }
}

void handle_inventory(GameState* gs) {
    if (gs->player.inventory_count == 0) {
        log_action(gs, "INFO", "Your inventory is empty.");
        return;
    }
    char buffer[MAX_LINE_LENGTH * 2] = "You are carrying: "; // Increased buffer
    for (int i = 0; i < gs->player.inventory_count; ++i) {
        strcat(buffer, "[");
        strcat(buffer, gs->items[gs->player.inventory[i]].name);
        strcat(buffer, "]");
        if (i < gs->player.inventory_count - 1) {
            strcat(buffer, ", ");
        }
    }
    log_action(gs, "INFO", buffer);
}

// --- Specific 'use' handlers ---
void handle_use_cook(GameState* gs) {
    if (gs->player.current_room_id != ROOM_GALLEY) {
        log_action(gs, "GAME_ERROR", "The cook isn't here."); return;
    }
    if (gs->cook_pleased) {
        log_action(gs, "GAME_INFO", "The cook nods. 'The stew was fine. Go on about yer business.'"); return;
    }
    if (player_has_item(&gs->player, ITEM_SALTED_PORK) &&
        player_has_item(&gs->player, ITEM_HARDTACK_BISCUITS) &&
        player_has_item(&gs->player, ITEM_GROG_BOTTLE)) {
        log_action(gs, "GAME_EVENT", "You give the Salted Pork, Hardtack Biscuits, and Grog Bottle to the Cook.");
        log_action(gs, "GAME_EVENT", "Cook: 'Hah! Not entirely useless, are ye? Right then, the Captain's stew it is! You can pass north now.'");
        gs->cook_pleased = 1;
        remove_item_from_inventory(&gs->player, ITEM_SALTED_PORK);
        remove_item_from_inventory(&gs->player, ITEM_HARDTACK_BISCUITS);
        remove_item_from_inventory(&gs->player, ITEM_GROG_BOTTLE);
    } else {
        log_action(gs, "GAME_EVENT", "Cook: 'That's not what I asked for, ya scallywag! Trying to poison the Captain, are ye?!'");
        handle_game_over(gs, "The Cook draws a cleaver and ends your adventure.", "GAME_OVER_COOK");
    }
}

void handle_use_sword_for_prisoners(GameState* gs) {
    if (!player_has_item(&gs->player, ITEM_SWORD)) {
        log_action(gs, "GAME_ERROR", "You fumble for a sword you don't have!"); // Should not happen if logic is right
        handle_game_over(gs, "Without a weapon to defend yourself, the prisoners overwhelm you.", "GAME_OVER_PRISONERS");
        return;
    }
    log_action(gs, "ACTION", "You draw your sword, its polished steel gleaming in the dim light.");
    log_action(gs, "GAME_EVENT", "The prisoners halt, eyes wide with fear. 'Alright, alright! We yield! No need for bloodshed!'");
    gs->prisoners_pacified = 1;
    gs->prisoners_hostile = 0;
    gs->special_prompt_active = 0;
    log_action(gs, "GAME_EVENT", "One prisoner speaks: 'We're tired of this brig... Answer our riddles three!'");
    gs->current_riddle_idx = 0;
    log_action(gs, "RIDDLE", "Riddle 1: I have a spine, but no bones. I tell tales of distant zones. What am I?");
    log_action(gs, "PROMPT", "Your answer? > ");
    gs->special_prompt_active = 1;
    strcpy(gs->special_prompt_context, "RIDDLE_ANSWER_1");
}

void handle_use_diary(GameState* gs) {
    if (!player_has_item(&gs->player, ITEM_DIARY)) {
        log_action(gs, "GAME_ERROR", "You don't have the diary."); return;
    }
    if (gs->diary_deciphered) {
        log_action(gs, "GAME_INFO", "You've already read the important parts. It mentioned 'Esmeralda' and the code 'ESMERALDA'."); return;
    }
    if (player_has_item(&gs->player, ITEM_MYSTIC_LENS)) {
        log_action(gs, "GAME_EVENT", "You hold the Mystic Lens over the diary. The strange symbols resolve into clear text!");
        log_action(gs, "GAME_EVENT", "Diary: 'My heart's true north, her name the key... What is her name?'");
        log_action(gs, "PROMPT", "Enter her name: > ");
        gs->special_prompt_active = 1;
        strcpy(gs->special_prompt_context, "DIARY_RIDDLE_ANSWER");
    } else {
        display_ascii_art("JUMBLED_DIARY");
        log_action(gs, "GAME_INFO", "You might need something special to decipher it.");
    }
}

void handle_use_treasurekey(GameState* gs) {
    if (!player_has_item(&gs->player, ITEM_TREASURE_KEY)) {
        log_action(gs, "GAME_ERROR", "You don't have the treasure key."); return;
    }
    if (gs->player.current_room_id == ROOM_CAPTAIN_QUARTERS && gs->rooms[ROOM_CAPTAIN_QUARTERS].exits[0] == ROOM_TREASURE_ROOM) {
        log_action(gs, "GAME_EVENT", "You try the Treasure Key on the locked door to the north... it seems like it would fit!");
        log_action(gs, "GAME_INFO", "(Try going north to use it automatically)");
    } else {
        log_action(gs, "GAME_INFO", "There's nothing here to use the treasure key on.");
    }
}

// Main 'use' dispatcher
void handle_use(GameState* gs, const char* item_name_str) {
    if (!item_name_str) { log_action(gs, "GAME_ERROR", "Use what?"); return; }

    if (strcasecmp(item_name_str, "cook") == 0) handle_use_cook(gs);
    else if (strcasecmp(item_name_str, "sword") == 0) {
        if (gs->special_prompt_active && strcmp(gs->special_prompt_context, "PRISONER_ATTACK") == 0 && gs->player.current_room_id == ROOM_PRISON_HOLD) {
            handle_use_sword_for_prisoners(gs);
        } else {
            log_action(gs, "GAME_INFO", "You swing your sword around. Impressive, but not very useful right now.");
        }
    }
    else if (strcasecmp(item_name_str, "diary") == 0) handle_use_diary(gs);
    else if (strcasecmp(item_name_str, "lens") == 0) {
        if (player_has_item(&gs->player, ITEM_DIARY) && player_has_item(&gs->player, ITEM_MYSTIC_LENS)) {
            handle_use_diary(gs); // Using lens on diary
        } else if (player_has_item(&gs->player, ITEM_MYSTIC_LENS)) {
            log_action(gs, "GAME_INFO", "You look through the lens. The world looks... shimmery.");
        } else {
            log_action(gs, "GAME_ERROR", "You don't have a lens to use.");
        }
    }
    else if (strcasecmp(item_name_str, "treasurekey") == 0) handle_use_treasurekey(gs);
    else if (strcasecmp(item_name_str, "portrait") == 0) {
         log_action(gs, "GAME_INFO", "You try to 'use' the portrait, but it just hangs there, staring sternly.");
    }
    else {
        char buffer[MAX_LINE_LENGTH];
        sprintf(buffer, "You can't use '%s' that way or it does nothing here.", item_name_str);
        log_action(gs, "GAME_ERROR", buffer);
    }
}

void handle_open_chest(GameState* gs) {
    if (gs->player.current_room_id != ROOM_CAPTAIN_QUARTERS) {
        log_action(gs, "GAME_ERROR", "There is no chest here to open."); return;
    }
    if (gs->chest_unlocked) {
        char chest_msg[MAX_LINE_LENGTH] = "The chest is already open. ";
        if (!player_has_item(&gs->player, ITEM_TREASURE_KEY) && item_in_room(&gs->rooms[ROOM_CAPTAIN_QUARTERS], ITEM_TREASURE_KEY)){
            strcat(chest_msg, "You see the [TreasureKey] inside.");
        } else if (player_has_item(&gs->player, ITEM_TREASURE_KEY)){
            strcat(chest_msg, "You already took the key.");
        } else {
             strcat(chest_msg, "It's empty now."); // Should not happen if key is only item
        }
        log_action(gs, "GAME_INFO", chest_msg);
        return;
    }
    if (!gs->knows_chest_code) {
        log_action(gs, "GAME_INFO", "The chest has a combination lock. You don't know the code."); return;
    }
    log_action(gs, "PROMPT", "The chest has a combination lock. Enter the code: > ");
    gs->special_prompt_active = 1;
    strcpy(gs->special_prompt_context, "CHEST_CODE_ENTRY");
}


// --- Special Input Processing ---
void process_special_input(GameState* gs, const char* raw_input) {
    char input[MAX_LINE_LENGTH];
    strncpy(input, raw_input, MAX_LINE_LENGTH -1);
    input[MAX_LINE_LENGTH-1] = '\0';
    to_lower_str(input); // Processed input is lowercase

    if (strcmp(gs->special_prompt_context, "PRISONER_ATTACK") == 0) {
        int used_correct_weapon = 0;
        // Input is already lowercased and includes the full command e.g., "use sword"
        if (strcmp(input, "use sword") == 0 && player_has_item(&gs->player, ITEM_SWORD)) {
            used_correct_weapon = 1;
        } else if (strcmp(input, "use knife") == 0 && player_has_item(&gs->player, ITEM_SWORD)) {
            // Player typed "use knife" and has a sword, which we treat as the intended weapon.
            // The game currently only has ITEM_SWORD, so "knife" acts as an alias here.
            used_correct_weapon = 1;
        }

        if (used_correct_weapon) {
            handle_use_sword_for_prisoners(gs); // This function will clear special_prompt_active
        } else {
            log_action(gs, "GAME_EVENT", "You fumble or try the wrong action!");
            handle_game_over(gs, "The prisoners don\'t wait for you to get your weapon right and overwhelm you.", "GAME_OVER_PRISONERS");
        }
    }
    else if (strncmp(gs->special_prompt_context, "RIDDLE_ANSWER_", strlen("RIDDLE_ANSWER_")) == 0) {
        int riddle_num = gs->current_riddle_idx;
        const char* answers_r1[] = {"book", "logbook", NULL};
        const char* answers_r2[] = {"horizon", NULL};
        const char* answers_r3[] = {"bottle", NULL};
        const char** current_answers = NULL;

        if (riddle_num == 0) current_answers = answers_r1;
        else if (riddle_num == 1) current_answers = answers_r2;
        else if (riddle_num == 2) current_answers = answers_r3;

        int correct = 0;
        if(current_answers) {
            for(int i=0; current_answers[i] != NULL; ++i) {
                if (strcmp(input, current_answers[i]) == 0) {
                    correct = 1;
                    break;
                }
            }
        }

        if (correct) {
            log_action(gs, "GAME_EVENT", "Correct!");
            gs->current_riddle_idx++;
            if (gs->current_riddle_idx >= 3) {
                log_action(gs, "GAME_EVENT", "Impressive! You've got a sharp mind. They give you a [Mystic Lens].");
                add_item_to_inventory(&gs->player, ITEM_MYSTIC_LENS);
                log_action(gs, "GAME_EVENT", "Prisoner: 'We heard the Captain scribbling in his diary... something about a secret.'");
                gs->special_prompt_active = 0;
            } else {
                if (gs->current_riddle_idx == 1) log_action(gs, "RIDDLE", "Riddle 2: I am always coming, but never arrive... What am I?");
                else if (gs->current_riddle_idx == 2) log_action(gs, "RIDDLE", "Riddle 3: What has a neck without a head... What am I?");
                log_action(gs, "PROMPT", "Your answer? > ");
                sprintf(gs->special_prompt_context, "RIDDLE_ANSWER_%d", gs->current_riddle_idx + 1);
            }
        } else {
            gs->riddle_attempts[riddle_num]--;
            if (gs->riddle_attempts[riddle_num] > 0) {
                char buffer[MAX_LINE_LENGTH];
                sprintf(buffer, "Nay, that's not it. Try again. (%d attempts left for this riddle)", gs->riddle_attempts[riddle_num]);
                log_action(gs, "GAME_EVENT", buffer);
                log_action(gs, "PROMPT", "Your answer? > ");
            } else {
                log_action(gs, "GAME_EVENT", "You've exhausted their patience with your foolish guesses!");
                handle_game_over(gs, "The prisoners lose their temper and attack.", "GAME_OVER_PRISONERS");
                gs->special_prompt_active = 0;
            }
        }
    }
    else if (strcmp(gs->special_prompt_context, "DIARY_RIDDLE_ANSWER") == 0) {
        if (strcmp(input, "esmeralda") == 0) {
            log_action(gs, "GAME_EVENT", "Correct! The diary pages shimmer: 'The code to my chest is her name: ESMERALDA'.");
            gs->diary_deciphered = 1;
            gs->knows_chest_code = 1;
            gs->special_prompt_active = 0;
        } else {
            log_action(gs, "GAME_EVENT", "That name doesn't seem right. The diary remains cryptic.");
            gs->special_prompt_active = 0;
        }
    }
    else if (strcmp(gs->special_prompt_context, "CHEST_CODE_ENTRY") == 0) {
        if (strcmp(input, "esmeralda") == 0) {
            log_action(gs, "GAME_EVENT", "CLICK! The lock springs open. Inside is a gleaming [TreasureKey]!");
            gs->chest_unlocked = 1;
            add_item_to_room(&gs->rooms[ROOM_CAPTAIN_QUARTERS], ITEM_TREASURE_KEY);
            gs->special_prompt_active = 0;
        } else {
            log_action(gs, "GAME_EVENT", "The lock remains stubbornly shut.");
            gs->special_prompt_active = 0;
        }
    }
    else if (strcmp(gs->special_prompt_context, "INTRO_CONTINUE") == 0) {
        if (strcmp(input, "continue") == 0) {
            log_action(gs, "ACTION", "You decide to board the galleon...");
            run_animation(ANIMATION_FILENAME, gs); // ANIMATION_FILENAME is from animation.h
            gs->player.current_room_id = ROOM_DECK;
            show_room_description(gs);
            gs->special_prompt_active = 0;
        } else {
            log_action(gs, "GAME_INFO", "Type 'continue' to start your adventure.");
            log_action(gs, "PROMPT", "> ");
        }
    }
}

void restart_game_flow(GameState* gs) {
    log_action(gs, "SYSTEM", "Restarting game / Starting new game...");
    init_game_state(gs); // Resets flags, player pos, inventory, room items etc.
    
    CLEAR_SCREEN();

    // Display the initial welcome and help message
    // Note: gs->log_file might not be open yet if this is the very first start
    // and log_action tries to use it. display_help_message handles gs being NULL.
    // However, in our main.c, log_file is opened before the first call to restart_game_flow.
    display_help_message(gs); // Show how to play

    // Then, the story intro
    log_action(gs, "STORY", "You are adrift in a small, rickety boat... A towering pirate galleon looms nearby. This might be your only chance...");
    log_action(gs, "PROMPT", "Type 'continue' to board the ship. > ");
    gs->special_prompt_active = 1;
    strcpy(gs->special_prompt_context, "INTRO_CONTINUE");
}

void handle_hint(GameState* gs) {
    Room* current_room = &gs->rooms[gs->player.current_room_id];
    char hint_message[MAX_LINE_LENGTH * 2] = ""; // Increased buffer for longer hints

    switch (gs->player.current_room_id) {
        case ROOM_DECK:
            strcat(hint_message, "This is where your adventure begins. Try 'look' to get your bearings, 'examine [item]' to inspect things closely (like that [parrot] or [portrait]), and 'pick up [item]' for useful objects like the [sword]. The only obvious exit is [East].");
            break;
        case ROOM_GALLEY:
            if (!gs->cook_pleased) {
                strcat(hint_message, "The Cook blocks the way North. He needs specific ingredients: [Salted Pork], [Hardtack Biscuits], and a [Grog Bottle]. These might be in the [Fridge] to the South. Once you have them all, try 'use cook'.");
            } else {
                strcat(hint_message, "The Cook is satisfied. The path North to the [Captain\'s Quarters] is open. You can also go [West] back to the Deck or [South] to the Fridge.");
            }
            break;
        case ROOM_FRIDGE:
            strcat(hint_message, "This pantry should have the ingredients the Cook needs. Look for [Salted Pork] (pork), [Hardtack Biscuits] (biscuits), and a [Grog Bottle] (grog). Use 'pick up [item]' for each.");
            break;
        case ROOM_CAPTAIN_QUARTERS:
            if (!gs->chest_unlocked) {
                if (!gs->diary_deciphered && player_has_item(&gs->player, ITEM_DIARY) && player_has_item(&gs->player, ITEM_MYSTIC_LENS)) {
                    strcat(hint_message, "You have the [Diary] and the [Mystic Lens]. Try 'use diary' to see if the lens reveals its secrets. The [Chest] in the corner also looks important.");
                } else if (!gs->diary_deciphered && player_has_item(&gs->player, ITEM_DIARY)) {
                    strcat(hint_message, "The Captain\'s [Diary] seems to be written in code. Perhaps an item from the prisoners could help? Don\'t forget about the [Chest].");
                } else if (gs->diary_deciphered && gs->knows_chest_code) {
                    strcat(hint_message, "You know the code 'ESMERALDA' for the [Chest] from the diary. Try to 'open chest'.");
                } else {
                    strcat(hint_message, "The Captain\'s Quarters... A [Diary] lies on the desk, and a [Chest] is in the corner. The North door to the [Treasure Room] is locked tight.");
                }
            } else if (gs->chest_unlocked && !player_has_item(&gs->player, ITEM_TREASURE_KEY) && item_in_room(current_room, ITEM_TREASURE_KEY)) {
                strcat(hint_message, "The [Chest] is open and you found the [TreasureKey]! Make sure to 'pick up treasurekey'.");
            } else if (player_has_item(&gs->player, ITEM_TREASURE_KEY)) {
                strcat(hint_message, "You have the [TreasureKey]! The door North to the [Treasure Room] should now unlock if you try to go 'north'.");
            } else {
                strcat(hint_message, "The North door to the [Treasure Room] is locked. Search for clues or a key. The [Chest] might be a good place to start if you can open it.");
            }
            break;
        case ROOM_PRISON_HOLD:
            if (gs->special_prompt_active && strcmp(gs->special_prompt_context, "PRISONER_ATTACK") == 0) {
                strcat(hint_message, "The prisoners are hostile! If you picked up the [Sword] from the deck, 'use sword' to defend yourself!");
            } else if (gs->special_prompt_active && strncmp(gs->special_prompt_context, "RIDDLE_ANSWER_", strlen("RIDDLE_ANSWER_")) == 0) {
                strcat(hint_message, "Answer the prisoners\' riddles. Type your one-word answer and press Enter.");
            } else if (player_has_item(&gs->player, ITEM_MYSTIC_LENS) && player_has_item(&gs->player, ITEM_DIARY) && !gs->diary_deciphered) {
                strcat(hint_message, "The prisoners gave you a [Mystic Lens]. If you also have the Captain\'s [Diary], try 'use diary' or 'use lens' to decipher it.");
            } else if (!player_has_item(&gs->player, ITEM_MYSTIC_LENS) && gs->prisoners_pacified) {
                 strcat(hint_message, "The prisoners mentioned a [Mystic Lens] after you solved their riddles. It should be in your inventory. It might be useful later.");
            } else {
                strcat(hint_message, "The prisoners are calm now. They mentioned something about the Captain\'s diary. The way [West] leads back to the Captain\'s Quarters.");
            }
            break;
        case ROOM_TREASURE_ROOM:
            strcat(hint_message, "You\'ve made it to the Treasure Room! Congratulations, you\'ve won!");
            break;
        default:
            strcat(hint_message, "No specific hint here. Try 'look' to survey your surroundings or 'inventory' to check your items.");
            break;
    }

    if (strlen(hint_message) > 0) {
        log_action(gs, "HINT", hint_message);
    } else {
        log_action(gs, "HINT", "No specific hint available right now.");
    }
}