#ifndef ITEM_H
#define ITEM_H

#include "common.h" // For MAX_LINE_LENGTH, GameState fwd dec

#define MAX_ITEMS 20 // Max different types of items

typedef enum {
    ITEM_SWORD,
    ITEM_PARROT,
    ITEM_PORTRAIT,
    ITEM_SALTED_PORK,
    ITEM_HARDTACK_BISCUITS,
    ITEM_GROG_BOTTLE,
    ITEM_ROTTEN_FISH,
    ITEM_DIARY,
    ITEM_MYSTIC_LENS,
    ITEM_TREASURE_KEY,
    ITEM_NONE // Represents no item, or end of list
} ItemID;

typedef struct Item {
    ItemID id;
    const char* name;
    const char* description; // General description
    const char* examine_text; // Detailed text for 'examine' command
    int can_pickup;
} Item;

// Function prototypes
void init_items(GameState* gs); // Now takes GameState to initialize gs->items
Item* get_item_by_name(GameState* gs, const char* name); // Now takes GameState to search gs->items

#endif // ITEM_H