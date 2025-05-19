#include "parser.h"
#include "utils.h" // for to_lower_str

#include <string.h> // for strtok, strcmp, strcspn
#include <stdio.h>  // for NULL

void parse_input(char* input, char** command, char** argument) {
    char* mutable_input = input; // use a pointer to make it clear strtok modifies it.
    to_lower_str(mutable_input);
    mutable_input[strcspn(mutable_input, "\n")] = 0; // remove newline

    *command = strtok(mutable_input, " \t\n");
    if (*command == NULL) {
        *argument = NULL;
        return;
    }

    // handle "pick up" as a single "pickup" command
    if (strcmp(*command, "pick") == 0) {
        char* next_token = strtok(NULL, " \t\n");
        if (next_token != NULL && strcmp(next_token, "up") == 0) {
            *command = "pickup"; // normalized command
            *argument = strtok(NULL, " \t\n");
        } else {
            *command = "pickup"; // treat "pick sword" as "pickup sword"
            *argument = next_token;
        }
    } else if (strcmp(*command, "take") == 0) { // also allow "take" as "pickup"
         *command = "pickup";
         *argument = strtok(NULL, " \t\n");
    }
    // handle "look at" as "examine"
    else if (strcmp(*command, "look") == 0) {
        char* next_token = strtok(NULL, " \t\n");
        if (next_token != NULL && strcmp(next_token, "at") == 0) {
            *command = "examine"; // normalize "look at" to "examine"
            *argument = strtok(NULL, " \t\n");
        } else {
            // if just "look", argument remains next_token (which could be NULL)
            // if "look <item>", command remains "look", argument is <item>
            // game logic will handle "look" with NULL arg as "look around"
            // and "look <item>" will be re-routed to examine by game logic
            *argument = next_token;
        }
    }
    // for other commands that take one argument
    else if (strcmp(*command, "drop") == 0 ||
               strcmp(*command, "examine") == 0 ||
               strcmp(*command, "use") == 0 ||
               strcmp(*command, "open") == 0) {
        *argument = strtok(NULL, " \t\n");
    }
    // for commands that take no arguments (N, S, E, W, inventory, quit, help, map, hint)
    else {
        *argument = NULL;
    }
}