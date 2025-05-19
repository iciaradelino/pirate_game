#ifndef UTILS_H
#define UTILS_H

#include "common.h" // For GameState forward dec, FILE*, MAX_LINE_LENGTH, etc.
#include "game_state.h" // Add if GameState is used by other utils funcs, or fine to have here

// Function prototypes
void log_action(GameState* gs, const char* action_type, const char* message);
void print_to_console(const char* message); // Simple wrapper if needed, or just use printf
void to_lower_str(char* str);
void display_ascii_art(const char* art_name);
void display_help_message(GameState* gs); // For logging
void display_map(GameState* gs); // Added for map display
char* get_absolute_path(const char* relative_path); // Returns absolute path from current directory
int open_file_with_default_app(const char* filename); // Opens a file with the system's default application

#endif // UTILS_H