#include "room.h"
#include <stdio.h> // For NULL

// Define the global rooms array
Room game_rooms[NUM_ROOMS];

void add_item_to_room(Room* room, ItemID item_id) {
    if (room->item_count < MAX_ROOM_ITEMS) {
        room->items_in_room[room->item_count++] = item_id;
    }
    // Else: could add an error message if room is full
}

void remove_item_from_room(Room* room, ItemID item_id) {
    int found_idx = -1;
    for (int i = 0; i < room->item_count; ++i) {
        if (room->items_in_room[i] == item_id) {
            found_idx = i;
            break;
        }
    }

    if (found_idx != -1) {
        // Shift items down to fill the gap
        for (int i = found_idx; i < room->item_count - 1; ++i) {
            room->items_in_room[i] = room->items_in_room[i + 1];
        }
        room->item_count--;
    }
}

// Definition for item_in_room
int item_in_room(Room* room, ItemID item_id) {
    for (int i = 0; i < room->item_count; ++i) {
        if (room->items_in_room[i] == item_id) {
            return 1; // True, item is in room
        }
    }
    return 0; // False, item is not in room
}


void init_rooms(void) {
    // ROOM_DECK
    game_rooms[ROOM_DECK] = (Room){ROOM_DECK, "Pirate Ship Deck",
        "You are on the weathered deck of the pirate ship. Salt spray mists the air. You see a weathered mast, coiled ropes, and a door leading further into the ship.",
        {NUM_ROOMS, NUM_ROOMS, ROOM_GALLEY, NUM_ROOMS}, // N, S, E, W (NUM_ROOMS means no exit)
        {ITEM_NONE}, 0, 0
    };
    add_item_to_room(&game_rooms[ROOM_DECK], ITEM_SWORD);
    add_item_to_room(&game_rooms[ROOM_DECK], ITEM_PARROT);
    add_item_to_room(&game_rooms[ROOM_DECK], ITEM_PORTRAIT);

    // ROOM_GALLEY
    game_rooms[ROOM_GALLEY] = (Room){ROOM_GALLEY, "The Galley",
        "The cramped galley smells of stale food and rum. A grumpy-looking Cook stirs a bubbling pot. There's a door to the south leading to a cold storage area, and another door north.",
        {ROOM_CAPTAIN_QUARTERS, ROOM_FRIDGE, NUM_ROOMS, ROOM_DECK},
        {ITEM_NONE}, 0, 0
    };

    // ROOM_FRIDGE
    game_rooms[ROOM_FRIDGE] = (Room){ROOM_FRIDGE, "Fridge Room (Pantry)",
        "A chilly room, lined with shelves. Various foodstuffs are stored here.",
        {ROOM_GALLEY, NUM_ROOMS, NUM_ROOMS, NUM_ROOMS},
        {ITEM_NONE}, 0, 0
    };
    add_item_to_room(&game_rooms[ROOM_FRIDGE], ITEM_SALTED_PORK);
    add_item_to_room(&game_rooms[ROOM_FRIDGE], ITEM_HARDTACK_BISCUITS);
    add_item_to_room(&game_rooms[ROOM_FRIDGE], ITEM_GROG_BOTTLE);
    add_item_to_room(&game_rooms[ROOM_FRIDGE], ITEM_ROTTEN_FISH);

    // ROOM_CAPTAIN_QUARTERS
    game_rooms[ROOM_CAPTAIN_QUARTERS] = (Room){ROOM_CAPTAIN_QUARTERS, "Captain's Quarters",
        "A lavishly decorated cabin. A large Captain figure lies snoring on a rug. A grand desk holds a leather-bound [Diary]. A sturdy [Chest] sits in the corner. There's a barred door to the east.",
        {ROOM_TREASURE_ROOM, ROOM_GALLEY, ROOM_PRISON_HOLD, NUM_ROOMS},
        {ITEM_NONE}, 0, 0
    };
    add_item_to_room(&game_rooms[ROOM_CAPTAIN_QUARTERS], ITEM_DIARY);

    // ROOM_PRISON_HOLD
    game_rooms[ROOM_PRISON_HOLD] = (Room){ROOM_PRISON_HOLD, "Prison Hold",
        "A dark, damp hold. Three shadowy Prisoners look agitated.",
        {NUM_ROOMS, NUM_ROOMS, NUM_ROOMS, ROOM_CAPTAIN_QUARTERS},
        {ITEM_NONE}, 0, 0
    };

    // ROOM_TREASURE_ROOM
    game_rooms[ROOM_TREASURE_ROOM] = (Room){ROOM_TREASURE_ROOM, "Treasure Room",
        "Piles of gold coins, chests overflowing with jewels, ancient artifacts, and all manner of glittering plunder fill the room!",
        {NUM_ROOMS, NUM_ROOMS, NUM_ROOMS, NUM_ROOMS},
        {ITEM_NONE}, 0, 0
    };
}