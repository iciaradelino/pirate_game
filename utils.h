#ifndef UTILS_H
#define UTILS_H

#include "common.h" // For GameState forward dec, FILE*, MAX_LINE_LENGTH

// Function prototypes
void log_action(GameState* gs, const char* action_type, const char* message);
void print_to_console(const char* message); // Simple wrapper if needed, or just use printf
void to_lower_str(char* str);
void display_ascii_art(const char* art_name);

#endif // UTILS_H