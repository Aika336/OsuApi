#pragma once

#include <string>

namespace logger {
	void Log(const std::string& msg);
	void LogError(const std::string& msg);
}

#define LOG(msg) logger::Log(msg)
#define LOG_ERROR(msg) logger::LogError(msg)