#include <iostream>
#include <OsuHandler.h>

int main() {

	OsuHandler handler;
	GameState g_state = handler.GetGameState();
	while (1) {
		handler.UpdateGameState();
		g_state = handler.GetGameState();

		if (g_state.is_playing) {
			std::cout << g_state.current_combo << std::endl;
			for (auto mods : g_state.current_mods) {
				std::cout << '[' << mods << ']';
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "Not playing" << std::endl;
		}
		Sleep(300);
		system("cls");
	}

    return 0;
}