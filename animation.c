#include "animation.h"
#include "utils.h" // for log_action, get_absolute_path
#include "game_state.h" // for full GameState for log_action

int run_animation(const char* filename, GameState* gs) {
    // get absolute path for the animation file
    char* abs_path = get_absolute_path(filename);
    
    FILE *fp = fopen(abs_path, "r");
    if (!fp) {
        char error_msg[MAX_LINE_LENGTH * 2];
        sprintf(error_msg, "Failed to open animation file: %s (File does not exist or cannot be accessed)", abs_path);
        log_action(gs, "SYSTEM_ERROR", error_msg);
        perror("fopen animation");
        return 1; // error
    }

    char line[MAX_LINE_LENGTH];
    char frame_buffer[MAX_FRAME_LINES * MAX_LINE_LENGTH] = {0};
    float delay_seconds;

    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return 1; } // read delay
    delay_seconds = atof(line);

    // skip header lines (assuming 3 lines after delay)
    for(int i = 0; i < 3; ++i) {
        if (!fgets(line, sizeof(line), fp)) { fclose(fp); return 1; }
    }
    
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0; // remove newline character

        if (strcmp(line, FRAME_DELIMITER) == 0) {
            CLEAR_SCREEN();
            printf("%s\n", frame_buffer);
            fflush(stdout);
            SLEEP_MS((int)(delay_seconds * 1000));
            frame_buffer[0] = '\0'; // clear buffer
        } else {
            strcat(frame_buffer, line);
            strcat(frame_buffer, "\n");
        }
    }
    
    fclose(fp);
    // last frame might be important, so we don't clear screen here
    return 0; // success
}