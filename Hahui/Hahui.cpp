#include <iostream>
#include "Reader.h"
#include "Reader.cpp"

int main() {
    OsuReader osu_reader;
    while (1) {
        osu_reader.ReadGameState();
        std::cout << osu_reader.GetLastGameState().is_playing << std::endl;
        Sleep(200);
        system("cls")
    }
}
