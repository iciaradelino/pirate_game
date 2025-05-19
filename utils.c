#include "common.h" // For MAP_FILENAME and other common definitions
#include "utils.h"
#include "game_state.h" // Needs full GameState definition for gs->log_file
#include <unistd.h> // For getcwd

// Function to get absolute path based on executable's directory
char* get_absolute_path(const char* relative_path) {
    static char abs_path[PATH_MAX]; // Use static buffer to avoid heap allocation
    static char exe_dir[PATH_MAX] = {0}; // Cache for executable directory
    static int first_run = 1; // Flag for first run checks
    
    // Only determine the executable directory once
    if (exe_dir[0] == '\0') {
        // Get current working directory
        if (getcwd(exe_dir, PATH_MAX) == NULL) {
            perror("getcwd");
            strcpy(exe_dir, "."); // Fallback to current directory
        }
        
        printf("DEBUG: Game running from directory: %s\n", exe_dir);
    }
    
    // Construct absolute path: exe_dir + "/" + relative_path
    snprintf(abs_path, PATH_MAX, "%s/%s", exe_dir, relative_path);
    
    // On first run, check if critical directories exist
    if (first_run) {
        char dir_check[PATH_MAX];
        snprintf(dir_check, PATH_MAX, "%s/ascii", exe_dir);
        FILE* test = fopen(dir_check, "r");
        if (!test) {
            printf("WARNING: Directory %s does not exist or cannot be accessed.\n", dir_check);
            printf("Make sure game assets are in the correct location relative to the executable.\n");
        } else {
            fclose(test);
            printf("DEBUG: Directory %s exists and is accessible\n", dir_check);
        }
        first_run = 0;
    }
    
    return abs_path;
}

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
    // Get absolute path for the map file
    char* abs_path = get_absolute_path(MAP_FILENAME);
    
    FILE *fp = fopen(abs_path, "r");
    if (!fp) {
        char err_msg[MAX_LINE_LENGTH + 100];
        sprintf(err_msg, "Failed to open map file: %s (File does not exist or cannot be accessed)", abs_path);
        log_action(gs, "SYSTEM_ERROR", err_msg);
        perror("fopen map");
        print_to_console("Map file not found or unreadable.");
        return;
    }

    char line[MAX_LINE_LENGTH * 2]; // Allow for potentially wider map lines
    while (fgets(line, sizeof(line), fp)) {
        // Don't use log_action for each line of the map, just print directly
        // to avoid cluttering the log and for direct console output.
        printf("%s", line); // fgets keeps the newline if space allows
    }
    fflush(stdout); // Ensure map is printed immediately

    fclose(fp);
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
        "    guide            - Open the detailed game guide in your default text viewer.\n"
        "    hint             - Get a hint for your current situation.\n"
        "    quit             - Exit the game.\n"
        "----------------------------------------------------------------------";

    // Log that help was displayed, but print the raw text for better formatting.
    if (gs) { // gs might be NULL if called before game state is fully ready for logging
        log_action(gs, "INFO", "Help message displayed to player.");
    }
    printf("%s\n", help_text);
}

// Opens a file with the system's default application
int open_file_with_default_app(const char* filename) {
    char *abs_path = get_absolute_path(filename);
    char command[PATH_MAX * 2]; // Double to account for quotes and command prefix
    int result = 0;
    
    // First, check if the file exists before trying to open it
    FILE *test_file = fopen(abs_path, "r");
    if (!test_file) {
        printf("Warning: Could not find guide file: %s\n", abs_path);
        return -1;
    }
    fclose(test_file);

    // Using system() can be problematic, especially on Windows, so we'll add
    // error handling and make the command construction more robust
#ifdef _WIN32
    // Windows: use 'start' to open with the default application
    // The empty quotes prevent issues with paths containing spaces
    snprintf(command, sizeof(command), "start \"\" \"%s\" >nul 2>&1", abs_path);
#elif defined(__APPLE__) || defined(__MACH__)
    // macOS: use 'open'
    snprintf(command, sizeof(command), "open \"%s\" >/dev/null 2>&1", abs_path);
#else
    // Linux/Unix: use 'xdg-open'
    snprintf(command, sizeof(command), "xdg-open \"%s\" >/dev/null 2>&1", abs_path);
#endif

    // Run the command in a way that won't block the game if it fails
    // We run it in the background to avoid hanging
#ifdef _WIN32
    // On Windows, system() returns the command's exit code
    result = system(command);
#else
    // On Unix-like systems, run the command in the background
    char bg_command[PATH_MAX * 2 + 10]; // Extra space for background operator
    snprintf(bg_command, sizeof(bg_command), "%s &", command);
    result = system(bg_command);
#endif
    
    // No need to free abs_path as it points to a static buffer in get_absolute_path
    
    return result;
}