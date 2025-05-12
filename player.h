#ifndef PLAYER_H
#define PLAYER_H

#include "common.h" // For basic types if needed
#include "item.h" // Needs ItemID
#include "room.h" // Needs RoomID

#define MAX_INVENTORY_ITEMS 10

typedef struct Player {
    RoomID current_room_id;
    ItemID inventory[MAX_INVENTORY_ITEMS];
    int inventory_count;
} Player;

// Function prototypes
void add_item_to_inventory(Player* player, ItemID item_id);
void remove_item_from_inventory(Player* player, ItemID item_id);
int player_has_item(Player* player, ItemID item_id);

#endif // PLAYER_H