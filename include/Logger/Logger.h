#pragma once

#include <string>

namespace logger {
	void Log(const std::string& msg);
	void LogError(const std::string& msg);
}

#define LOG(msg) logger::Log(std::string(__FUNCSIG__) + ": " + msg)
#define LOG_ERROR(msg) logger::LogError(std::string(__FUNCSIG__) + ": " + msg)