# Pirate Adventure Text Game

A simple text-based adventure game written in C.

## Game Logic & Puzzle Overview

The player starts adrift and must board a pirate ship. The goal is to find the Captain's treasure. The game features ASCII art for various scenes and events.

**Flow: (contains spoilers!)**

1.  **Intro:** Player types `continue` to swing aboard the ship (ASCII animation displayed from `swing_animation_frames.txt`).
2.  **Deck (ROOM_DECK):** Player starts here. Can find a `sword`, a `parrot`. Can `examine` a `portrait` (reveals "Esmeralda").
    - **Important:** The `parrot` must be dropped before entering the Captain's Quarters, or it will wake the Captain, leading to a game over.
3.  **Galley (ROOM_GALLEY):** The Cook demands ingredients: `pork`, `biscuits`, `grog`. These must be fetched from the `Fridge Room`.
    - **Fridge Room (ROOM_FRIDGE):** Contains the ingredients and a red herring (`fish`).
      - **Timer:** Player has 30 seconds (FRIDGE_TIME_LIMIT) to pick up items and leave the Fridge. Staying too long results in game over (hypothermia).
    - Give correct items to Cook (`use cook`) to proceed north to Captain's Quarters. Failure results in game over.
4.  **Captain's Quarters (ROOM_CAPTAIN_QUARTERS):**
    - Entering with the `parrot` in inventory wakes the Captain, resulting in game over.
    - Contains a `diary` (pick up) and a locked `chest`.
    - **Captain's Riddle for Prison Hold Access:** To go West to the Prison Hold, if the riddle isn\'t solved yet, the player is presented with a riddle: "Silent servant by my side, Through roiled seas and turning tide, With face unblinking, ever true, It guides my hand when night is blue." (Answer: `compass`). The player has 3 attempts. Failure means the door remains locked.
5.  **Prison Hold (ROOM_PRISON_HOLD - West of Captain's Quarters):**
    - On entry (if prisoners are hostile), prisoners attack. Player must `use sword` to pacify them. Failure results in game over.
    - Pacified prisoners present three riddles. Player gets 3 tries per riddle.
      1.  Riddle 1: "I have a spine, but no bones. I tell tales of distant zones. What am I?" (Answer: `book` or `logbook`)
      2.  Riddle 2: "I am always coming, but never arrive. I am always seen, but never alive. Men chart their course by my distant dive. What am I?" (Answer: `horizon`)
      3.  Riddle 3: "What has a neck without a head, a body without legs, and is often found where pirates are dead (or just where they tread)?" (Answer: `bottle`)
    - Failing riddles results in game over. Success yields a `mystic lens`.
6.  **Diary & Chest Puzzle (Captain's Quarters):**
    - `use diary` with the `mystic lens` in inventory deciphers it.
    - The diary poses a riddle: "What is her name?" (Answer: `Esmeralda`, from the portrait).
    - Correct answer reveals the chest code: `ESMERALDA`.
    - `open chest` and enter the code `Esmeralda` (case-insensitive) to unlock it.
    - The chest contains the `treasurekey` (player must `pick up treasurekey`).
7.  **Treasure Room (ROOM_TREASURE_ROOM - North of Captain's Quarters):**
    - Requires the `treasurekey` to enter (either `use treasurekey` or automatic on attempting to move north).
    - Entering this room wins the game.

**Key Commands:**

- `north`, `south`, `east`, `west` (or `n`, `s`, `e`, `w`)
- `look` (describes room)
- `look at [item]` or `examine [item]`
- `pick up [item]` or `take [item]`
- `drop [item]`
- `inventory` or `i`
- `use [item/character]` (e.g., `use cook`, `use sword`, `use diary`)
- `open [item]` (e.g., `open chest`)
- `help` (displays command list)
- `map` (displays the game map from `map.txt`)
- `hint` (provides a context-sensitive hint)
- `guide` (attempts to open `game_guide.txt`)
- `quit`

## Compilation instructions

The game uses a `Makefile` for easier compilation.

1.  Ensure you have a C compiler like `gcc` installed.
2.  Open your terminal or command prompt.
3.  Navigate to the root directory of the project (where the `Makefile` is located).
4.  Run the command:
    Mac - `sh make`
    Windows - `mingw32-make` (if using mingw32 compiler)

    This will compile all necessary C files (`.c`) into object files (`.o`) in a `dist/` directory, and then link them to create the executable `dist/pirate_adventure` (or `dist/pirate_adventure.exe` on Windows).
    The `Makefile` also copies resource files (like ASCII art and the game guide) into the `dist/` directory.

To clean up compiled files and the `dist/` directory:
Mac - ` sh make clean `
Windows - `mingw32-make clean`

## Running the Game

After successful compilation:

1.  Navigate to the `dist` directory:
    ```sh    cd dist     ```
2.  Execute the compiled program:
    - On Linux/macOS: `./pirate_adventure`
    - On Windows: `pirate_adventure.exe` or `.\pirate_adventure.exe`

The `game_log.txt` file will be created/appended in the directory from which the game is executed, logging game events and player commands.

## Notes

- Player input is generally case-insensitive.
- Some puzzles have game-over conditions if failed.
- The game supports restarting after a game over.
- The game attempts to open `game_guide.txt` in an external application at the start and when the `guide` command is used.
