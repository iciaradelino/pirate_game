#include "room.h"
#include "game_state.h" // needed for init_rooms signature
#include <stdio.h> // for NULL

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
        // shift items down to fill the gap
        for (int i = found_idx; i < room->item_count - 1; ++i) {
            room->items_in_room[i] = room->items_in_room[i + 1];
        }
        room->item_count--;
    }
}

// definition for item_in_room
int item_in_room(Room* room, ItemID item_id) {
    for (int i = 0; i < room->item_count; ++i) {
        if (room->items_in_room[i] == item_id) {
            return 1; // True, item is in room
        }
    }
    return 0; // False, item is not in room
}


void init_rooms(GameState* gs) {
    // ROOM_DECK
    gs->rooms[ROOM_DECK] = (Room){ROOM_DECK, "Pirate Ship Deck",
        "You stand on the weathered deck of a pirate galleon. Salt spray mists the air. You see a tall, weathered mast with coiled ropes, a ship\'s wheel, and a sturdy door leading [East] further into the ship. A [Sword] lies discarded near the railing, a colorful [Parrot] squawks from atop a barrel, and a faded [Portrait] is nailed to the mast.",
        {NUM_ROOMS, NUM_ROOMS, ROOM_GALLEY, NUM_ROOMS}, // N, S, E, W (NUM_ROOMS means no exit)
        {ITEM_NONE}, 0, 0
    };
    add_item_to_room(&gs->rooms[ROOM_DECK], ITEM_SWORD);
    add_item_to_room(&gs->rooms[ROOM_DECK], ITEM_PARROT);
    add_item_to_room(&gs->rooms[ROOM_DECK], ITEM_PORTRAIT);

    // ROOM_GALLEY
    gs->rooms[ROOM_GALLEY] = (Room){ROOM_GALLEY, "The Galley",
        "The cramped galley smells of stale food and potent rum. A grumpy-looking Cook stirs a bubbling pot, eyeing you suspiciously. A door [North] seems to lead to the Captain\'s private area, another door [South] likely goes to a fridge, and the way [West] returns to the Deck.",
        {ROOM_CAPTAIN_QUARTERS, ROOM_FRIDGE, NUM_ROOMS, ROOM_DECK},
        {ITEM_NONE}, 0, 0
    };

    // ROOM_FRIDGE
    gs->rooms[ROOM_FRIDGE] = (Room){ROOM_FRIDGE, "Fridge",
        "A chilly fridge, lined with shelves. Various foodstuffs are stored here, including [Salted Pork], [Hardtack Biscuits], a [Grog Bottle], and some less appetizing [Rotten Fish]. The only exit is [North] back to the Galley.",
        {ROOM_GALLEY, NUM_ROOMS, NUM_ROOMS, NUM_ROOMS}, // N, S, E, W - North to Galley
        {ITEM_NONE}, 0, 0
    };
    add_item_to_room(&gs->rooms[ROOM_FRIDGE], ITEM_SALTED_PORK);
    add_item_to_room(&gs->rooms[ROOM_FRIDGE], ITEM_HARDTACK_BISCUITS);
    add_item_to_room(&gs->rooms[ROOM_FRIDGE], ITEM_GROG_BOTTLE);
    add_item_to_room(&gs->rooms[ROOM_FRIDGE], ITEM_ROTTEN_FISH);

    // ROOM_CAPTAIN_QUARTERS
    gs->rooms[ROOM_CAPTAIN_QUARTERS] = (Room){ROOM_CAPTAIN_QUARTERS, "Captain\'s Quarters",
        "A surprisingly lavishly decorated cabin. A large Captain figure lies snoring loudly on a rug. A grand desk holds a leather-bound [Diary]. A sturdy, locked [Chest] sits in the corner. A barred door [West] suggests a [Prison Hold], while the exit [South] leads back to the Galley. A heavy door to the [North] is probably the way to the [Treasure Room], but it\'s securely locked.",
        {ROOM_TREASURE_ROOM, ROOM_GALLEY, NUM_ROOMS, ROOM_PRISON_HOLD},
        {ITEM_NONE}, 0, 0
    };
    add_item_to_room(&gs->rooms[ROOM_CAPTAIN_QUARTERS], ITEM_DIARY);

    // ROOM_PRISON_HOLD
    gs->rooms[ROOM_PRISON_HOLD] = (Room){ROOM_PRISON_HOLD, "Prison Hold",
        "A dark, damp, and altogether unpleasant hold. Three shadowy Prisoners look up as you enter, their eyes glinting in the gloom. The only way out seems to be [East] back to the Captain\'s Quarters.",
        {NUM_ROOMS, NUM_ROOMS, ROOM_CAPTAIN_QUARTERS, NUM_ROOMS},
        {ITEM_NONE}, 0, 0
    };

    // ROOM_TREASURE_ROOM
    gs->rooms[ROOM_TREASURE_ROOM] = (Room){ROOM_TREASURE_ROOM, "Treasure Room",
        "Success! Piles of gold coins, chests overflowing with jewels, ancient artifacts, and all manner of glittering plunder fill this magnificent room! You\'ve found the pirate\'s treasure!",
        {NUM_ROOMS, NUM_ROOMS, NUM_ROOMS, NUM_ROOMS},
        {ITEM_NONE}, 0, 0
    };
}