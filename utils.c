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