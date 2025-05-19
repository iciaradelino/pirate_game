#ifndef ROOM_H
#define ROOM_H

#include "common.h"
#include "item.h"

#define MAX_ROOM_ITEMS     10
#define NUM_DIRECTIONS     4

typedef enum
{
    ROOM_DECK            = 0,
    ROOM_GALLEY          = 1,
    ROOM_FRIDGE          = 2,
    ROOM_CAPTAIN_QUARTERS= 3,
    ROOM_PRISON_HOLD     = 4,
    ROOM_TREASURE_ROOM   = 5,
    NUM_ROOMS            = 6
} RoomID;

typedef struct
{
    RoomID      id;
    const char *name;
    const char *description;
    RoomID      exits[NUM_DIRECTIONS];
    ItemID      items_in_room[MAX_ROOM_ITEMS];
    int         item_count;
    int         visited;
} Room;

void init_rooms            (GameState *gs);
void add_item_to_room      (Room *room, ItemID item_id);
void remove_item_from_room (Room *room, ItemID item_id);
int  item_in_room          (Room *room, ItemID item_id);

#endif // ROOM_H