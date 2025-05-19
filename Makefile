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
ifeq ($(OS),Windows_NT)
	cmd /c (if not exist dist mkdir dist) && (if not exist dist\ascii mkdir dist\ascii)
else
	mkdir -p dist/ascii
endif

copy_resources: | dist
ifeq ($(OS),Windows_NT)
	cmd /c xcopy /E /I /Y ascii dist\ascii
else
	cp -r ascii/* dist/ascii/
endif

clean:
ifeq ($(OS),Windows_NT)
	cmd /c if exist dist rd /S /Q dist
else
	rm -rf dist
endif

.PHONY: all clean