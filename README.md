```sh
gcc main.c -o game -Wall -Wextra -std=c99
```

latest.


# Pirate Adventure Text Game

A simple text-based adventure game written in C.

## Game Logic & Puzzle Overview

The player starts adrift and must board a pirate ship. The goal is to find the Captain's treasure.

**Flow:**
1.  **Intro:** Player types `continue` to swing aboard the ship (ASCII animation).
2.  **Deck (ROOM1):** Player can find a `sword`, a `parrot`, and `examine` a `portrait` (reveals "Esmeralda").
3.  **Galley (ROOM2):** The Cook demands ingredients: `pork`, `biscuits`, `grog`. These must be fetched from the `Fridge Room`.
    *   **Fridge Room (Sub-Room):** Contains the ingredients and a red herring (`fish`).
    *   Give correct items to Cook (`use cook`) to proceed north to Captain's Quarters. Failure results in game over.
4.  **Captain's Quarters (ROOM3):**
    *   Entering with the `parrot` in inventory wakes the Captain, resulting in game over.
    *   Contains a `diary` (pick up) and a locked `chest`.
5.  **Prison Hold (ROOM3B - East of Captain's Quarters):**
    *   On entry, prisoners attack. Player must `use sword` to pacify them. Failure results in game over.
    *   Pacified prisoners present three riddles. Player gets 3 tries per riddle.
        1.  Riddle 1: "I have a spine, but no bones. I tell tales of distant zones. What am I?" (Answer: `book` or `logbook`)
        2.  Riddle 2: "I am always coming, but never arrive. I am always seen, but never alive. Men chart their course by my distant dive. What am I?" (Answer: `horizon`)
        3.  Riddle 3: "What has a neck without a head, a body without legs, and is often found where pirates are dead (or just where they tread)?" (Answer: `bottle`)
    *   Failing riddles results in game over. Success yields a `mystic lens`.
6.  **Diary & Chest Puzzle (Captain's Quarters):**
    *   `use diary` with the `mystic lens` in inventory deciphers it.
    *   The diary poses a riddle: "What is her name?" (Answer: `Esmeralda`, from the portrait).
    *   Correct answer reveals the chest code: `ESMERALDA`.
    *   `open chest` and enter the code `Esmeralda` to unlock it.
    *   The chest contains the `treasurekey` (player must `pick up treasurekey`).
7.  **Treasure Room (ROOM4 - North of Captain's Quarters):**
    *   Requires the `treasurekey` to enter (either `use treasurekey` or automatic on attempting to move north).
    *   Entering this room wins the game.

**Key Commands:**
*   `north`, `south`, `east`, `west` (or `n`, `s`, `e`, `w`)
*   `look` (describes room)
*   `look at [item]` or `examine [item]`
*   `pick up [item]` or `take [item]`
*   `drop [item]`
*   `inventory` or `i`
*   `use [item/character]` (e.g., `use cook`, `use sword`, `use diary`)
*   `open [item]` (e.g., `open chest`)
*   `help`
*   `quit`

## Compilation

See compilation instructions provided (e.g., `gcc pirate_adventure.c -o pirate_adventure -Wall`).

## Running the Game

Execute the compiled program (e.g., `./pirate_adventure` or `pirate_adventure.exe`).
A `game_log.txt` file will be created/appended with game events and player commands.
An animation file `swing_animation_frames.txt` is required for the intro.

## Notes
* Input is generally case-insensitive.
* Some puzzles have game-over conditions if failed.
* The game supports restarting after a game over.