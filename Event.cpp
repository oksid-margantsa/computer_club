#include "Event.hpp"
#include <sstream>

Event::Event(const std::string& time, EventType type, const std::vector< std::string >& args):
  time(time),
  type(type),
  args(args)
{}

const std::string& Event::getTime() const
{
  return time;
}

EventType Event::getType() const
{
  return type;
}

const std::vector< std::string >& Event::getArgs() const
{
  return args;
}

std::string Event::toString() const
{
  std::ostringstream oss;
  oss << time << " " << static_cast< int >(type);
  for (const auto& arg : args)
  {
    oss << " " << arg;
  }
  return oss.str();
}