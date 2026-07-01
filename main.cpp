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
