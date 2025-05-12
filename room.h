#ifndef ROOM_H
#define ROOM_H

#include "common.h" // Needs GameState fwd dec
#include "item.h" // Needs ItemID

#define MAX_ROOM_ITEMS 10
#define NUM_DIRECTIONS 4 // North, South, East, West

typedef enum {
    ROOM_DECK,
    ROOM_GALLEY,
    ROOM_FRIDGE,
    ROOM_CAPTAIN_QUARTERS,
    ROOM_PRISON_HOLD,
    ROOM_TREASURE_ROOM,
    NUM_ROOMS // Count of actual rooms, also used as invalid/no exit
} RoomID;

typedef struct Room {
    RoomID id;
    const char* name;
    const char* description;
    RoomID exits[NUM_DIRECTIONS]; // N, S, E, W -> links to other RoomIDs
    ItemID items_in_room[MAX_ROOM_ITEMS];
    int item_count;
    int visited; // To control full vs. short description
} Room;

// Function prototypes
void init_rooms(GameState* gs); // Now takes GameState to initialize gs->rooms
void add_item_to_room(Room* room, ItemID item_id);
void remove_item_from_room(Room* room, ItemID item_id);
int item_in_room(Room* room, ItemID item_id); // Declaration for the fix

#endif // ROOM_H