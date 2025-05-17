#include "common.h" // For MAP_FILENAME and other common definitions
#include "utils.h"
#include "game_state.h" // Needs full GameState definition for gs->log_file

void log_action(GameState* gs, const char* action_type, const char* message) {
    time_t now = time(NULL);
    char time_str[30];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    if (gs->log_file) {
        fprintf(gs->log_file, "[%s] %s: %s\n", time_str, action_type, message);
        fflush(gs->log_file);
    }
    printf("%s\n", message); // Also print to console for immediate feedback
}

void print_to_console(const char* message) {
    printf("%s\n", message);
}

void to_lower_str(char* str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void display_ascii_art(const char* art_name) {
    if (strcmp(art_name, "GAME_OVER_COOK") == 0) {
        print_to_console("\n   _____          __  __ ______    ______      ________ _____  \n  / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ \n | |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |\n | | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  / \n | |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\ \n  \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\\n");
        print_to_console("The Cook didn't appreciate your offering. You've been skewered!");
    } else if (strcmp(art_name, "GAME_OVER_CAPTAIN") == 0) {
        print_to_console("\n   _____          __  __ ______    ______      ________ _____  \n  / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ \n | |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |\n | | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  / \n | |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\ \n  \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\\n");
        print_to_console("The Captain was not pleased to be woken by a squawking parrot! BANG!");
    } else if (strcmp(art_name, "GAME_OVER_PRISONERS") == 0) {
        print_to_console("\n   _____          __  __ ______    ______      ________ _____  \n  / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ \n | |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |\n | | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  / \n | |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\ \n  \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\\n");
        print_to_console("The prisoners were not impressed. You've met a sticky end.");
    } else if (strcmp(art_name, "WIN_GAME") == 0) {
        print_to_console("\n__   __            _    _ _       _ \n\\ \\ / /           | |  | (_)     | |\n \\ V /___  _   _  | |  | |_ _ __ | |\n  \\ // _ \\| | | | | |/\\| | | '_ \\| |\n  | | (_) | |_| | \\  /\\  / | | | |_|\n  \\_/\\___/ \\__,_|  \\/  \\/|_|_| |_(_)\n");
        print_to_console("Congratulations! You have plundered the pirate's loot and won the game!");
    } else if (strcmp(art_name, "JUMBLED_DIARY") == 0) {
        print_to_console("~@#$%^&*()_+|}{P ريال OIUYTREWQASDFGHJKL:NBVCXZ`1234567890-=");
        print_to_console("The script is a swirling, unreadable mess of arcane symbols.");
    }
}

void display_map(GameState* gs) {
    FILE *fp = fopen(MAP_FILENAME, "r");
    if (!fp) {
        char err_msg[MAX_LINE_LENGTH + 100];
        sprintf(err_msg, "Failed to open map file (%s).", MAP_FILENAME);
        log_action(gs, "SYSTEM_ERROR", err_msg);
        perror("fopen map");
        print_to_console("Map file not found or unreadable.");
        return;
    }

    char line[MAX_LINE_LENGTH * 2]; // Allow for potentially wider map lines
    print_to_console("\n--- MAP ---");
    while (fgets(line, sizeof(line), fp)) {
        // Don't use log_action for each line of the map, just print directly
        // to avoid cluttering the log and for direct console output.
        printf("%s", line); // fgets keeps the newline if space allows
    }
    print_to_console("-----------");
    fflush(stdout); // Ensure map is printed immediately

    fclose(fp);
    if (gs) { // Check if gs is valid before logging
      log_action(gs, "INFO", "Map displayed to player.");
    }
}

void display_help_message(GameState* gs) {
    const char* help_text =
        "----------------------------------------------------------------------\n"
        "Welcome to The Buccaneer\'s Cipher!\n"
        "Your goal is to explore the pirate ship, solve puzzles, and find the treasure.\n"
        "\n"
        "HOW TO PLAY:\n"
        " - Type commands and press Enter.\n"
        " - Commands are usually 1 or 2 words (e.g., \'look\', \'pick up sword\').\n"
        " - Pay attention to item names in room descriptions, often shown in [brackets].\n"
        "\n"
        "BASIC COMMANDS:\n"
        "  Movement:\n"
        "    north (or n)     - Move north\n"
        "    south (or s)     - Move south\n"
        "    east (or e)      - Move east\n"
        "    west (or w)      - Move west\n"
        "  Interaction:\n"
        "    look             - Describe your current room and visible items/exits.\n"
        "    look at [item]   - Examine an item more closely (e.g., \'look at sword\').\n"
        "    examine [item]   - Same as \'look at [item]\'.\n"
        "    pick up [item]   - Add an item to your inventory (e.g., \'pick up key\').\n"
        "    take [item]      - Same as \'pick up [item]\'.\n"
        "    drop [item]      - Remove an item from inventory and place it in the room.\n"
        "    inventory (or i) - Show what items you are carrying.\n"
        "    use [item]       - Use an item, sometimes on another object or character\n"
        "                       (e.g., \'use key\', \'use cook\', \'use diary\').\n"
        "    open [item]      - Try to open something (e.g., \'open chest\').\n"
        "    map              - Display the game map.\n"
        "  Game:\n"
        "    help             - Display this help message again.\n"
        "    hint             - Get a hint for your current situation.\n"
        "    quit             - Exit the game.\n"
        "----------------------------------------------------------------------";

    // Log that help was displayed, but print the raw text for better formatting.
    if (gs) { // gs might be NULL if called before game state is fully ready for logging
        log_action(gs, "INFO", "Help message displayed to player.");
    }
    printf("%s\n", help_text);
}