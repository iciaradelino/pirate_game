#include "animation.h"
#include "utils.h" // For log_action (needs full GameState)
#include "game_state.h" // For full GameState for log_action

int run_animation(const char *filename, GameState* gs) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        log_action(gs, "SYSTEM_ERROR", "Failed to open animation file.");
        perror("fopen animation");
        return 1; // Error
    }

    char line[MAX_LINE_LENGTH];
    char frame_buffer[MAX_FRAME_LINES * MAX_LINE_LENGTH] = {0};
    float delay_seconds;

    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return 1; } // Read delay
    delay_seconds = atof(line);

    // Skip header lines (assuming 3 lines after delay)
    for(int i=0; i<3; ++i) {
        if (!fgets(line, sizeof(line), fp)) { fclose(fp); return 1; }
    }
    
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0; // Remove newline

        if (strcmp(line, FRAME_DELIMITER) == 0) {
            CLEAR_SCREEN();
            printf("%s\n", frame_buffer);
            fflush(stdout);
            SLEEP_MS((int)(delay_seconds * 1000));
            frame_buffer[0] = '\0'; // Clear buffer
        } else {
            strcat(frame_buffer, line);
            strcat(frame_buffer, "\n");
        }
    }
    
    fclose(fp);
    // Last frame might be important, so don't clear screen here without thought
    return 0; // Success
}