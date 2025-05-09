CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =
OBJ_FILES = main.o item.o room.o player.o game_state.o utils.o animation.o parser.o game_logic.o
TARGET = pirate_adventure

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET) game_log.txt

.PHONY: all clean