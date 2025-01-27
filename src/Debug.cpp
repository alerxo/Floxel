#include "Debug.h"

#include <iostream>
#include <sstream>
#include <format>
#include <cmath>
#include <ctime>
#include <SDL3/SDL_filesystem.h>
#include <fstream>

std::ofstream file;
time_t startTime;

void Debug::Initialize()
{
	startTime = time(0);
	file.open(std::format("{}/log_file.txt", SDL_GetBasePath()), std::fstream::out);
	ASSERT(file.is_open(), true, "Unable to open log file.");
	file.clear();
	LOG("Debug file opened.");
}

void Debug::Destroy()
{
	LOG("Debug file closed.");
	file.close();
}

static std::string GetTimeStamp()
{
	double seconds = difftime(time(0), startTime);
	double minutes = seconds / 60;
	seconds = std::floor(std::fmod(seconds, 60));
	double hours = std::floor(minutes / 60);
	minutes = std::floor(std::fmod(minutes, 60));

	return std::format("[{}{}:{}{}:{}{}] ",
					   hours > 9 ? "" : "0", hours,
					   minutes > 9 ? "" : "0", minutes,
					   seconds > 9 ? "" : "0", seconds);
}

void Debug::Print(std::string message)
{
	std::ostringstream entry;
	entry << GetTimeStamp() << message << std::endl;

#ifdef DEBUG
	std::cout << entry.str();
#endif // DEBUG

	ASSERT(file.is_open(), true, "Log file is closed.");
	file << entry.str();
}