#include "Logger.h"

#include <iostream>

namespace Logger {
	void Log(const std::string& msg) {
		std::cout << msg << std::endl;
	}

	void Log(const std::string& msg) {
		std::cerr << msg << std::endl;
	}
}