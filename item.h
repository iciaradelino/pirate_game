#ifndef ITEM_H
#define ITEM_H

#include "common.h" // for MAX_LINE_LENGTH, GameState fwd dec

#define MAX_ITEMS 20 // max different types of items in inventory

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
    ITEM_NONE // represents no item, or end of list
} ItemID;

typedef struct Item {
    ItemID id;
    const char* name;
    const char* description; // general description
    const char* examine_text; // detailed text for 'examine' command
    int can_pickup;
} Item;

// function prototypes
void init_items(GameState* gs);
Item* get_item_by_name(GameState* gs, const char* name);

#endif // ITEM_H