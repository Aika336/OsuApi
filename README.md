# OsuApi basic information
***OsuApi*** - is a program designed to read data from the osu!Lazer. It uses the Windows API (WinApi).

## Technical points:
The application is based on WinApi, which we use to connect to the process and read data using offsets. From the very beginning, we need to find the base address where the game process itself runs. We use this address (which is static during the process) to find the required values ​​using offsets. The main class used to read data from the game is OsuReader.

When using the ReadGameState() method of an OsuReader instance, we will save all states in the private variable **game_state_**, which we can later retrieve by calling the **GetLastGameState()** method, which will return a GameState structure.
GameState is a structure that contains all the information obtained during the last call to the ReadGameState() method.

---

## Example:
To use the code, I can provide you with one very simple example.

```cpp
#include <iostream>
#include "Includes.h"

int main() {
    OsuReader osu_reader;
    
    GameState state = osu_reader.GetLastGameState();
    while (1) {
		osu_reader.ReadGameState();
		state = osu_reader.GetLastGameState();
        if (state.is_playing) {
            std::cout << state.mods << std::endl; // return current enable mods
            std::cout << state.combo << std::endl; // return current combo
        }
        else {
            std::cout << "Not playing" << std::endl;
        }
        Sleep(200);
        system("cls");
    }
}

```

This example checks whether the map is playing, and based on this, displays the current combo and a list of mods, or displays information if the player is not playing.

---

## Features of work:
The search for the base address from which the entire search will proceed is performed by searching for a signature:
```cpp
inline const std::vector<uint8_t> signature_ = {
    0x67, 0x00, 0x20, 0x00, 0x6F, 0x00, 0x6E, 0x00,
    0x20, 0x00, 0x34, 0x00, 0x35, 0x00, 0x33, 0x00,
    0x35, 0x00, 0x37, 0x00, 0x29, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
};
```
To find the address, use the **OsuReader::FindGameBaseAddress()** function, which internally searches the process's address pages and searches for a signature using the **OsuReader::CheckForStableSignature()** helper function.
**OsuReader::CheckForStableSignature()** - This method accepts a list of bytes located on a memory page and then compares and searches for the required byte within it using signature_

---

## Plans for the future:
 - [ ] Adding more information that can be read and applied.
 - [x] Migrate this project to CMake.
 - [ ] Adding a script that will automatically search for offsets.
