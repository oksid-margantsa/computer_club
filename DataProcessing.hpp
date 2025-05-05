#ifndef DATAPROCESSING_HPP
#define DATAPROCESSING_HPP
#include <string>
#include <cstdlib>
#include <stdexcept>

size_t timeFormatToMinutes(const std::string& timeStr);
std::string minutesInTimeFormat(size_t minutes);
bool isValidTimeFormat(const std::string& time);

#endif
