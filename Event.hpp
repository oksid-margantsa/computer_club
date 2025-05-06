#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <vector>
#include <sstream>

enum class EventType
{
  CLIENT_ARRIVED = 1,
  CLIENT_SAT = 2,
  CLIENT_WAITING = 3,
  CLIENT_LEFT = 4,
  CLIENT_FORCED_LEFT = 11,
  CLIENT_SAT_FROM_QUEUE = 12,
  ERROR = 13
};

class Event
{
public:
  Event(const std::string& time, EventType type, const std::vector< std::string >& args = {});

  const std::string& getTime() const;
  EventType getType() const;
  const std::vector< std::string >& getArgs() const;
  std::string toString() const;

private:
  std::string time;
  EventType type;
  std::vector< std::string > args;
};

#endif