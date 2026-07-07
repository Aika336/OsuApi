#include "Logger.h"

#include <iostream>

namespace logger {
	void Log(const std::string& msg) {
		std::cout << msg << std::endl;
	}

	void LogError(const std::string& msg) {
		std::cerr << msg << std::endl;
	}
}