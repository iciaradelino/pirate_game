#include "parser.h"
#include "utils.h"

#include <string.h>
#include <stdio.h>

void parse_input(char *input, char **command, char **argument)
{
    char *mutable_input = input;
    to_lower_str(mutable_input);
    mutable_input[strcspn(mutable_input, "\n")] = 0;

    *command = strtok(mutable_input, " \t\n");
    if (*command == NULL)
    {
        *argument = NULL;
        return;
    }

    if (strcmp(*command, "pick") == 0)
    {
        char *next_token = strtok(NULL, " \t\n");
        if (next_token != NULL && strcmp(next_token, "up") == 0)
        {
            *command  = "pickup";
            *argument = strtok(NULL, " \t\n");
        }
        else
        {
            *command  = "pickup";
            *argument = next_token;
        }
    }
    else if (strcmp(*command, "take") == 0)
    {
        *command  = "pickup";
        *argument = strtok(NULL, " \t\n");
    }
    else if (strcmp(*command, "look") == 0)
    {
        char *next_token = strtok(NULL, " \t\n");
        if (next_token != NULL && strcmp(next_token, "at") == 0)
        {
            *command  = "examine";
            *argument = strtok(NULL, " \t\n");
        }
        else
        {
            *argument = next_token;
        }
    }
    else if (strcmp(*command, "drop")    == 0 ||
             strcmp(*command, "examine") == 0 ||
             strcmp(*command, "use")     == 0 ||
             strcmp(*command, "open")    == 0)
    {
        *argument = strtok(NULL, " \t\n");
    }
    else
    {
        *argument = NULL;
    }
}