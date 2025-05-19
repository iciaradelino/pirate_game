# Pirate Adventure Text Game

A simple text-based adventure game written in C.

## Game Logic & Puzzle Overview

The player starts adrift and must board a pirate ship. The goal is to find the Captain's treasure. The game features ASCII art for various scenes and events.

**Flow: (contains spoilers!)**

## Example Command Walkthrough (Spoilers!)

1.  `continue`  
    *Starts the game and boards the ship (ASCII animation plays).*

2.  `pick up sword`  
    *On the Deck. The sword is needed later in the Prison Hold.*

3.  `examine portrait`  
    *On the Deck. Reveals the name "Esmeralda", which is crucial for a later puzzle.*

4.  `east`  
    *Move from the Deck to the Galley.*

5.  `south`  
    *Move from the Galley to the Fridge Room.*

6.  `pick up pork`  
    *In the Fridge Room.*

7.  `pick up biscuits`  
    *In the Fridge Room.*

8.  `pick up grog`  
    *In the Fridge Room.*

9.  `north`  
    *Return to the Galley. Must be done within 30 seconds or you lose (hypothermia).*

10. `use cook`  
    *Give the ingredients to the Cook in the Galley.*

11. `north`  
    *Move from the Galley to the Captain's Quarters.*

12. `pick up diary`  
    *In the Captain's Quarters.*

13. `west`  
    *Attempt to move to the Prison Hold. Triggers the Captain's riddle.*

14. `compass`  
    *Answer to the Captain's riddle to unlock the door.*

15. `west`  
    *Enter the Prison Hold.*

16. `use sword`  
    *Pacify the prisoners when they attack.*

17. `book`  
    *Answer to the prisoners' first riddle. (`logbook` may also work.)*

18. `horizon`  
    *Answer to the prisoners' second riddle.*

19. `bottle`  
    *Answer to the prisoners' third riddle. This yields the mystic lens.*

20. `east`  
    *Return to the Captain's Quarters.*

21. `use diary`  
    *With the mystic lens in inventory, this deciphers the diary and presents a new riddle.*

22. `esmeralda`  
    *Answer to the diary's riddle: "What is her name?"*

23. `open chest`  
    *Interact with the locked chest in the Captain's Quarters.*

24. `esmeralda`  
    *Enter the code to unlock the chest, revealed by solving the diary riddle.*

25. `pick up treasurekey`  
    *From the now-unlocked chest in the Captain's Quarters.*

26. `north`  
    *Move from the Captain's Quarters to the Treasure Room, using the treasurekey. This wins the game.*

**Note:**  
- Do not pick up the parrot, or if you do, drop it before entering the Captain's Quarters, or you will lose the game.
- The order of commands above is the optimal solution path.

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
- `guide` (attempts to open `game_guide.txt`, only working for mac)
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
