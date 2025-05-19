#include "common.h"
#include "item.h"
#include "room.h"
#include "player.h"
#include "game_state.h"
#include "utils.h"
#include "parser.h"
#include "game_logic.h"
#include <stdio.h>
#include <string.h>

static int  handle_game_over_prompt (GameState *gs);
static int  process_player_input    (GameState *gs);
static int  execute_command         (GameState *gs, const char *command, const char *argument);

static FILE *open_log_file_checked(const char *filename)
{
    char *abs_path = get_absolute_path(filename);
    FILE *file = fopen(abs_path, "a");
    if (!file)
    {
        perror("Error opening log file");
        printf("Warning: Could not open log file %s. Game will continue without logging.\n", abs_path);
    }
    return file;
}

int main(void)
{
    GameState gs; // Create gs first

    // Open log file *before* any logging happens
    gs.log_file = open_log_file_checked(LOG_FILENAME);
    if (gs.log_file)
    {
        log_action(&gs, "SYSTEM", "Log file opened.");
    }
    log_action(&gs, "SYSTEM", "Game client started.");

    // Initialize the static item definitions *into gs*
    init_items(&gs); // Pass gs

    // Try to open the game guide with the system's default text editor
    // We wrap this in a try-catch style block to prevent crashes
    {
        log_action(&gs, "SYSTEM", "Attempting to open game guide...");
        
        int guide_open_result = open_file_with_default_app(GUIDE_FILENAME);
        if (guide_open_result != 0) {
            // If opening the guide failed, log it but continue with the game
            log_action(&gs, "SYSTEM_WARNING", "Failed to open game guide. Type 'help' to see commands.");
            printf("Note: You can view the game guide by typing 'guide' during gameplay.\n");
        } else {
            log_action(&gs, "SYSTEM", "Game guide opened in external application.");
        }
    }

    restart_game_flow(&gs); // This calls init_game_state which calls init_rooms

    while (!gs.game_won)
    {
        int continue_game = 1;

        if (gs.game_over)
        {
            int restart = handle_game_over_prompt(&gs);
            if (restart)
            {
                restart_game_flow(&gs);
            }
            else
            {
                continue_game = 0;
            }
        }
        else
        {
            // Check fridge timer before processing player input
            check_fridge_timer(&gs);
            if (!gs.game_over) {  // Only process input if game isn't over from timer
                continue_game = process_player_input(&gs);
            }
        }

        if (!continue_game)
        {
            break;
        }
    }

    if (gs.game_won)
    {
        log_action(&gs, "SYSTEM", "Game finished with a win.");
    }
    else
    {
        log_action(&gs, "SYSTEM", "Game finished (Quit or Game Over without restart).");
    }

    log_action(&gs, "SYSTEM", "Game client ended.");
    if (gs.log_file)
    {
        fclose(gs.log_file);
    }

    return 0;
}

static int handle_game_over_prompt(GameState *gs)
{
    char input_buffer[MAX_LINE_LENGTH];

    if (gs->should_restart)
    {
        log_action(gs, "PROMPT", "Game Over. Play again? (yes/no) > ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin))
        {
            char temp_input[MAX_LINE_LENGTH];
            strncpy(temp_input, input_buffer, MAX_LINE_LENGTH - 1);
            temp_input[MAX_LINE_LENGTH - 1] = '\0';
            temp_input[strcspn(temp_input, "\n")] = 0;
            log_action(gs, "PLAYER_CMD (RESTART?)", temp_input);

            to_lower_str(input_buffer);
            input_buffer[strcspn(input_buffer, "\n")] = 0;

            if (strcmp(input_buffer, "yes") == 0)
            {
                return 1;
            }
            else
            {
                log_action(gs, "SYSTEM", "Thanks for playing!");
                return 0;
            }
        }
        else
        {
            log_action(gs, "SYSTEM", "Input error during restart prompt. Exiting.");
            return 0;
        }
    }

    log_action(gs, "SYSTEM_ERROR", "Game Over state reached unexpectedly without restart flag.");
    return 0;
}

static int process_player_input(GameState *gs)
{
    char input_buffer[MAX_LINE_LENGTH];

    if (!gs->special_prompt_active)
    {
        printf("\nWhat do you do? > ");
        fflush(stdout);
    }

    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
    {
        log_action(gs, "SYSTEM_ERROR", "Error reading input or EOF reached. Exiting.");
        return 0;
    }

    char logged_input[MAX_LINE_LENGTH];
    strncpy(logged_input, input_buffer, MAX_LINE_LENGTH - 1);
    logged_input[MAX_LINE_LENGTH - 1] = '\0';
    logged_input[strcspn(logged_input, "\n")] = 0;

    if (gs->special_prompt_active)
    {
        log_action(gs, "PLAYER_CMD (SPECIAL)", logged_input);
        process_special_input(gs, logged_input);
        return 1;
    }
    else
    {
        log_action(gs, "PLAYER_CMD", logged_input);

        char temp_parse_buffer[MAX_LINE_LENGTH];
        char *command  = NULL;
        char *argument = NULL;
        strncpy(temp_parse_buffer, input_buffer, MAX_LINE_LENGTH);
        temp_parse_buffer[MAX_LINE_LENGTH - 1] = '\0';
        parse_input(temp_parse_buffer, &command, &argument);

        if (command == NULL || strlen(command) == 0)
        {
            return 1;
        }
        return execute_command(gs, command, argument);
    }
}

static int execute_command(GameState *gs, const char *command, const char *argument)
{
    if (strcmp(command, "quit") == 0)
    {
        log_action(gs, "SYSTEM", "Quitting game.");
        return 0;
    }
    else if (strcmp(command, "north") == 0 || strcmp(command, "n") == 0)
    {
        handle_move(gs, "north");
    }
    else if (strcmp(command, "south") == 0 || strcmp(command, "s") == 0)
    {
        handle_move(gs, "south");
    }
    else if (strcmp(command, "east") == 0 || strcmp(command, "e") == 0)
    {
        handle_move(gs, "east");
    }
    else if (strcmp(command, "west") == 0 || strcmp(command, "w") == 0)
    {
        handle_move(gs, "west");
    }
    else if (strcmp(command, "look") == 0)
    {
        if (argument == NULL)
            handle_examine(gs, NULL);
        else
            handle_examine(gs, argument);
    }
    else if (strcmp(command, "examine") == 0)
    {
        if (!argument)
            log_action(gs, "GAME_ERROR", "Examine what?");
        else
            handle_examine(gs, argument);
    }
    else if (strcmp(command, "pickup") == 0)
    {
        handle_pickup(gs, argument);
    }
    else if (strcmp(command, "drop") == 0)
    {
        handle_drop(gs, argument);
    }
    else if (strcmp(command, "inventory") == 0 || strcmp(command, "i") == 0)
    {
        handle_inventory(gs);
    }
    else if (strcmp(command, "use") == 0)
    {
        handle_use(gs, argument);
    }
    else if (strcmp(command, "open") == 0 && argument && strcmp(argument, "chest") == 0)
    {
        handle_open_chest(gs);
    }
    else if (strcmp(command, "help") == 0)
    {
        display_help_message(gs);
    }
    else if (strcmp(command, "map") == 0)
    {
        display_map(gs);
    }
    else if (strcmp(command, "hint") == 0)
    {
        handle_hint(gs);
    }
    else if (strcmp(command, "guide") == 0)
    {
        log_action(gs, "INFO", "Attempting to open game guide...");
        
        // Use a safer approach to open the guide
        int guide_open_result = open_file_with_default_app(GUIDE_FILENAME);
        if (guide_open_result != 0) {
            log_action(gs, "GAME_ERROR", "Could not open the game guide. Using in-game help instead.");
            // Fall back to displaying the help message if opening the guide fails
            display_help_message(gs);
        } else {
            log_action(gs, "INFO", "Game guide opened in external application.");
        }
    }
    else
    {
        char err_buf[MAX_LINE_LENGTH];
        sprintf(err_buf, "Unknown command: '%s'", command ? command : "(empty)");
        log_action(gs, "GAME_ERROR", err_buf);
    }

    return 1;
}