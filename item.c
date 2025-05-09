#include "item.h"
#include <string.h> // For strcasecmp
#include <stdio.h>  // For NULL if not in common.h for some reason

// Define the global items array
Item game_items[MAX_ITEMS];

void init_items(void) {
    game_items[ITEM_SWORD] =             (Item){ITEM_SWORD, "sword", "A trusty pirate sword.", "A sharp, gleaming cutlass. Looks reliable.", 1};
    game_items[ITEM_PARROT] =            (Item){ITEM_PARROT, "parrot", "A colorful, noisy parrot.", "It keeps muttering 'Pieces of eight! Pieces of eight!'", 1};
    game_items[ITEM_PORTRAIT] =          (Item){ITEM_PORTRAIT, "portrait", "A gaudy portrait.", "A faded portrait of a stern-looking woman. A small plaque beneath reads 'Esmeralda, his guiding star.'", 0};
    game_items[ITEM_SALTED_PORK] =       (Item){ITEM_SALTED_PORK, "pork", "A slab of salted pork.", "Looks tough, but probably edible. Salted pork.", 1};
    game_items[ITEM_HARDTACK_BISCUITS] = (Item){ITEM_HARDTACK_BISCUITS, "biscuits", "Some hardtack biscuits.", "Rock-hard biscuits. Hardtack biscuits, a pirate staple.", 1};
    game_items[ITEM_GROG_BOTTLE] =       (Item){ITEM_GROG_BOTTLE, "grog", "A bottle of grog.", "A dark bottle filled with what smells like strong grog.", 1};
    game_items[ITEM_ROTTEN_FISH] =       (Item){ITEM_ROTTEN_FISH, "fish", "A rotten fish.", "Ugh, this fish smells terrible. Definitely rotten.", 1};
    game_items[ITEM_DIARY] =             (Item){ITEM_DIARY, "diary", "A leather-bound diary.", "A heavy, locked diary. It seems important.", 1};
    game_items[ITEM_MYSTIC_LENS] =       (Item){ITEM_MYSTIC_LENS, "lens", "A mystic lens.", "A peculiar lens that seems to shimmer with an inner light.", 1};
    game_items[ITEM_TREASURE_KEY] =      (Item){ITEM_TREASURE_KEY, "treasurekey", "A large ornate key.", "This must be the key to the treasure!", 1};
    game_items[ITEM_NONE] =              (Item){ITEM_NONE, "", "", "", 0}; // Sentinel
}

Item* get_item_by_name(const char* name) {
    if (name == NULL) return NULL;
    for (int i = 0; game_items[i].id != ITEM_NONE; ++i) {
        if (strcasecmp(game_items[i].name, name) == 0) {
            return &game_items[i];
        }
    }
    return NULL;
}