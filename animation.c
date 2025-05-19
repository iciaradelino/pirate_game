#include "animation.h"
#include "utils.h"
#include "game_state.h"

int run_animation(const char *filename, GameState *gs)
{
    char   *abs_path     = get_absolute_path(filename);
    FILE   *fp           = fopen(abs_path, "r");
    char    line[MAX_LINE_LENGTH];
    char    frame_buffer[MAX_FRAME_LINES * MAX_LINE_LENGTH] = {0};
    float   delay_seconds;

    if (!fp)
    {
        char error_msg[MAX_LINE_LENGTH * 2];
        sprintf(error_msg, "Failed to open animation file: %s (File does not exist or cannot be accessed)", abs_path);
        log_action(gs, "SYSTEM_ERROR", error_msg);
        perror("fopen animation");
        return 1;
    }

    if (!fgets(line, sizeof(line), fp))
    {
        fclose(fp);
        return 1;
    }
    delay_seconds = atof(line);

    for (int i = 0; i < 3; ++i)
    {
        if (!fgets(line, sizeof(line), fp))
        {
            fclose(fp);
            return 1;
        }
    }

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, FRAME_DELIMITER) == 0)
        {
            CLEAR_SCREEN();
            printf("%s\n", frame_buffer);
            fflush(stdout);
            SLEEP_MS((int)(delay_seconds * 1000));
            frame_buffer[0] = '\0';
        }
        else
        {
            strcat(frame_buffer, line);
            strcat(frame_buffer, "\n");
        }
    }

    fclose(fp);
    return 0;
}