#include "common.h"
#include "item.h"
#include "room.h"
#include "player.h" // Not directly used in main, but good to show gs includes it
#include "game_state.h"
#include "utils.h"
#include "parser.h"
#include "game_logic.h"
// animation.h is included via game_logic.h for restart_game_flow

int main() {
    GameState gs;
    char input_buffer[MAX_LINE_LENGTH];
    char* command;
    char* argument;

    // Initialize game components
    init_items();
    // init_rooms(); // This is called within init_game_state now
    // init_game_state(&gs); // This is called by restart_game_flow

    gs.log_file = fopen(LOG_FILENAME, "a"); // Open log file in append mode
    if (!gs.log_file) {
        perror("Error opening log file");
        // Game can proceed without logging, but good to note
        // return 1; // Or allow game to run without log
    }
    log_action(&gs, "SYSTEM", "Game client started.");

    restart_game_flow(&gs); // Sets up initial game state and intro

    while (!gs.game_won) {
        if (gs.game_over) {
            if (gs.should_restart) { // Check if restart is pending
                log_action(&gs, "PROMPT", "Game Over. Play again? (yes/no) > ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
                    char temp_input[MAX_LINE_LENGTH]; // for logging raw input
                    strncpy(temp_input, input_buffer, MAX_LINE_LENGTH -1);
                    temp_input[MAX_LINE_LENGTH-1] = '\0';
                    temp_input[strcspn(temp_input, "\n")] = 0;
                    log_action(&gs, "PLAYER_CMD (RESTART?)", temp_input);

                    to_lower_str(input_buffer); // process for "yes"/"no"
                    input_buffer[strcspn(input_buffer, "\n")] = 0;

                    if (strcmp(input_buffer, "yes") == 0) {
                        restart_game_flow(&gs);
                        // game_over will be reset by init_game_state via restart_game_flow
                        // should_restart will also be reset
                        continue; // Back to the game loop start
                    } else {
                        log_action(&gs, "SYSTEM", "Thanks for playing!");
                        break; // Exit while loop, end game
                    }
                } else { // EOF or error
                    log_action(&gs, "SYSTEM", "Input error during restart prompt. Exiting.");
                    break;
                }
            } else { // game_over is true, but not prompting for restart (should not happen with current logic)
                 break; // Safety break
            }
        }

        if (!gs.special_prompt_active) {
            printf("\nWhat do you do? > "); // Raw printf for prompt to avoid logging it repeatedly as "INFO"
            // log_action(&gs, "PROMPT", "\nWhat do you do? > "); // Avoids double print if log_action also prints
        }
        // Special prompts are printed by the functions that set them.

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            log_action(&gs, "SYSTEM_ERROR", "Error reading input or EOF reached. Exiting.");
            break; 
        }
        
        char logged_input[MAX_LINE_LENGTH]; 
        strncpy(logged_input, input_buffer, MAX_LINE_LENGTH-1);
        logged_input[MAX_LINE_LENGTH-1] = '\0';
        logged_input[strcspn(logged_input, "\n")] = 0; 

        if (gs.special_prompt_active) {
            log_action(&gs, "PLAYER_CMD (SPECIAL)", logged_input);
            process_special_input(&gs, logged_input);
        } else {
            log_action(&gs, "PLAYER_CMD", logged_input);
            char temp_parse_buffer[MAX_LINE_LENGTH]; // strtok modifies string, so pass a copy
            strncpy(temp_parse_buffer, input_buffer, MAX_LINE_LENGTH);
            temp_parse_buffer[MAX_LINE_LENGTH-1] = '\0';

            parse_input(temp_parse_buffer, &command, &argument);

            if (command == NULL || strlen(command) == 0) { // Empty input
                continue;
            }

            if (strcmp(command, "quit") == 0) {
                log_action(&gs, "SYSTEM", "Quitting game.");
                break;
            } else if (strcmp(command, "north") == 0 || strcmp(command, "n") == 0) {
                handle_move(&gs, "north");
            } else if (strcmp(command, "south") == 0 || strcmp(command, "s") == 0) {
                handle_move(&gs, "south");
            } else if (strcmp(command, "east") == 0 || strcmp(command, "e") == 0) {
                handle_move(&gs, "east");
            } else if (strcmp(command, "west") == 0 || strcmp(command, "w") == 0) {
                handle_move(&gs, "west");
            } else if (strcmp(command, "look") == 0) { // "look" or "look <item>"
                 if (argument == NULL) handle_examine(&gs, NULL); // look around
                 else handle_examine(&gs, argument); // "look <item>" -> examine <item>
            } else if (strcmp(command, "examine") == 0) {
                 if (!argument) log_action(&gs, "GAME_ERROR", "Examine what?");
                 else handle_examine(&gs, argument);
            } else if (strcmp(command, "pickup") == 0) { // "pickup <item>" or "take <item>"
                handle_pickup(&gs, argument);
            } else if (strcmp(command, "drop") == 0) {
                handle_drop(&gs, argument);
            } else if (strcmp(command, "inventory") == 0 || strcmp(command, "i") == 0) {
                handle_inventory(&gs);
            } else if (strcmp(command, "use") == 0) {
                handle_use(&gs, argument);
            } else if (strcmp(command, "open") == 0 && argument && strcmp(argument, "chest") == 0) {
                handle_open_chest(&gs);
            } else if (strcmp(command, "help") == 0) {
                display_help_message(&gs); // Call the comprehensive help display
            }
            else {
                char err_buf[MAX_LINE_LENGTH];
                sprintf(err_buf, "Unknown command: '%s'", command ? command : "(empty)");
                log_action(&gs, "GAME_ERROR", err_buf);
            }
        }
    }

    if (gs.game_won) {
        log_action(&gs, "SYSTEM", "Congratulations on winning!");
    }
    log_action(&gs, "SYSTEM", "Game client ended.");
    if (gs.log_file) {
        fclose(gs.log_file);
    }

    return 0;
}