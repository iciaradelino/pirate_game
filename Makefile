CC        = gcc
CFLAGS    = -Wall -Wextra -std=c99
LDFLAGS   =
OBJ_FILES = dist/main.o dist/item.o dist/room.o dist/player.o dist/game_state.o dist/utils.o dist/animation.o dist/parser.o dist/game_logic.o
TARGET    = dist/pirate_adventure

all: $(TARGET) copy_resources

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

dist/%.o: %.c | dist
	$(CC) $(CFLAGS) -c $< -o $@

dist:
	mkdir -p dist/ascii

copy_resources: | dist
	cp -r ascii/* dist/ascii/

clean:
ifeq ($(OS),Windows_NT)
	cmd /c del /F /Q dist\\*.o dist\\pirate_adventure dist\\game_log.txt
else
	rm -f dist/*.o dist/pirate_adventure dist/game_log.txt
endif

.PHONY: all clean