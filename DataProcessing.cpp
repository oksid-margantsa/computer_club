#include "DataProcessing.hpp"
#include <sstream>

std::vector< std::string > split(const std::string& s, char delimiter)
{
  std::vector< std::string > tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}

Event parseEvent(const std::string& line)
{
  auto parts = split(line, ' ');
  if (parts.size() < 2 || parts[0].empty() || parts[1].empty())
  {
    throw std::invalid_argument("Invalid event format");
  }

  std::string time = parts[0];
  EventType type = static_cast< EventType >(std::stoi(parts[1]));
  std::vector< std::string > args(parts.begin() + 2, parts.end());

  return Event(time, type, args);
}

bool isValidTimeFormat(const std::string& time)
{
  if (time.length() != 5) return false;
  if (time[2] != ':') return false;

  char* end;
  size_t hours = std::strtoul(time.substr(0, 2).c_str(), &end, 10);

  if (*end != '\0') return false;

  size_t minutes = std::strtoul(time.substr(3, 2).c_str(), &end, 10);

  if (*end != '\0') return false;
  if (hours > 23) return false;
  if (minutes > 59) return false;

  return true;
}

size_t timeFormatToMinutes(const std::string& timeStr)
{
  if (!isValidTimeFormat(timeStr))
  {
    throw std::invalid_argument("< Invalid time format >");
  }

  char* end;
  size_t hours = std::strtoul(timeStr.substr(0, 2).c_str(), &end, 10);
  size_t minutes = std::strtoul(timeStr.substr(3, 2).c_str(), &end, 10);

  return hours * 60 + minutes;
}

std::string minutesInTimeFormat(size_t minutes)
{
  int h = minutes / 60;
  int m = minutes % 60;
  char buf[6];
  std::sprintf(buf, "%02d:%02d", h, m);
  return std::string(buf);
}