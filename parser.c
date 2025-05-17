#include "parser.h"
#include "utils.h" // For to_lower_str

#include <string.h> // For strtok, strcmp, strcspn
#include <stdio.h>  // For NULL

void parse_input(char* input, char** command, char** argument) {
    char* mutable_input = input; // Use a pointer to make it clear strtok modifies it.
    to_lower_str(mutable_input);
    mutable_input[strcspn(mutable_input, "\n")] = 0; // Remove newline

    *command = strtok(mutable_input, " \t\n");
    if (*command == NULL) {
        *argument = NULL;
        return;
    }

    // Handle "pick up" as a single "pickup" command
    if (strcmp(*command, "pick") == 0) {
        char* next_token = strtok(NULL, " \t\n");
        if (next_token != NULL && strcmp(next_token, "up") == 0) {
            *command = "pickup"; // Normalized command
            *argument = strtok(NULL, " \t\n");
        } else {
            *command = "pickup"; // Treat "pick sword" as "pickup sword"
            *argument = next_token;
        }
    } else if (strcmp(*command, "take") == 0) { // Also allow "take" as "pickup"
         *command = "pickup";
         *argument = strtok(NULL, " \t\n");
    }
    // Handle "look at" as "examine"
    else if (strcmp(*command, "look") == 0) {
        char* next_token = strtok(NULL, " \t\n");
        if (next_token != NULL && strcmp(next_token, "at") == 0) {
            *command = "examine"; // Normalize "look at" to "examine"
            *argument = strtok(NULL, " \t\n");
        } else {
            // If just "look", argument remains next_token (which could be NULL)
            // If "look <item>", command remains "look", argument is <item>
            // Game logic will handle "look" with NULL arg as "look around"
            // and "look <item>" will be re-routed to examine by game logic
            *argument = next_token;
        }
    }
    // For other commands that take one argument
    else if (strcmp(*command, "drop") == 0 ||
               strcmp(*command, "examine") == 0 ||
               strcmp(*command, "use") == 0 ||
               strcmp(*command, "open") == 0) {
        *argument = strtok(NULL, " \t\n");
    }
    // For commands that take no arguments (N, S, E, W, inventory, quit, help, map)
    else {
        *argument = NULL; // Ensure argument is NULL for these
    }
}