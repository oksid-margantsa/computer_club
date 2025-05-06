#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <vector>

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
  Event(const std::string& t, EventType ty, const std::vector< std::string >& a = {}) :
    time(t),
    type(ty),
    args(a)
  {}

  const std::string& getTime() const { return time; }
  EventType getType() const { return type; }
  const std::vector<std::string>& getArgs() const { return args; }

private:
  std::string time;
  EventType type;
  std::vector< std::string > args;
};

#endif