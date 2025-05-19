#ifndef PLAYER_H
#define PLAYER_H

#include "common.h" // for basic types if needed
#include "item.h" // needs ItemID
#include "room.h" // needs RoomID

#define MAX_INVENTORY_ITEMS 10

typedef struct Player {
    RoomID current_room_id;
    ItemID inventory[MAX_INVENTORY_ITEMS];
    int inventory_count;
} Player;

// function prototypes
void add_item_to_inventory(Player* player, ItemID item_id);
void remove_item_from_inventory(Player* player, ItemID item_id);
int player_has_item(Player* player, ItemID item_id);

#endif // PLAYER_H