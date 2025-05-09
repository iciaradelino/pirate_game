#include "player.h"
#include <stdio.h> // For NULL

void add_item_to_inventory(Player* player, ItemID item_id) {
    if (player->inventory_count < MAX_INVENTORY_ITEMS) {
        player->inventory[player->inventory_count++] = item_id;
    }
    // Else: could add error for full inventory
}

void remove_item_from_inventory(Player* player, ItemID item_id) {
    int found_idx = -1;
    for (int i = 0; i < player->inventory_count; ++i) {
        if (player->inventory[i] == item_id) {
            found_idx = i;
            break;
        }
    }

    if (found_idx != -1) {
        for (int i = found_idx; i < player->inventory_count - 1; ++i) {
            player->inventory[i] = player->inventory[i + 1];
        }
        player->inventory_count--;
    }
}

int player_has_item(Player* player, ItemID item_id) {
    for (int i = 0; i < player->inventory_count; ++i) {
        if (player->inventory[i] == item_id) {
            return 1; // True
        }
    }
    return 0; // False
}