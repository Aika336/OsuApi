#pragma once

#include <string>

namespace Logger {
	void Log(const std::string& msg);
	void LogError(const std::string& msg);
}

#define LOG(msg) Logger::Log(msg)
#define LOG_ERROR(msg) Logger::LogError(msg)