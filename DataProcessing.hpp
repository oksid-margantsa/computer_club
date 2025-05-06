#ifndef DATAPROCESSING_HPP
#define DATAPROCESSING_HPP
#include <string>
#include <cstdlib>
#include <vector>
#include "Event.hpp"

size_t timeFormatToMinutes(const std::string& timeStr);
std::string minutesInTimeFormat(size_t minutes);
bool isValidTimeFormat(const std::string& time);
std::vector< std::string > split(const std::string& s, char delimiter);
Event parseEvent(const std::string& line);

#endif