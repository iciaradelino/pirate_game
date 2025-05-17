CC        = gcc
CFLAGS    = -Wall -Wextra -std=c99
LDFLAGS   =
OBJ_FILES = dist/main.o dist/item.o dist/room.o dist/player.o dist/game_state.o dist/utils.o dist/animation.o dist/parser.o dist/game_logic.o
TARGET    = dist/pirate_adventure

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

dist/%.o: %.c | dist
	$(CC) $(CFLAGS) -c $< -o $@

dist:
	mkdir -p dist

clean:
	del /F /Q dist\\*.o dist\\pirate_adventure dist\\game_log.txt

.PHONY: all clean