#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>

class Client
{
public:
  Client();
  Client(std::string clientName, size_t tableNumber, size_t timeMinutes);
  Client(const Client& other) noexcept = default;
  Client(Client&& other) noexcept = default;
  ~Client() = default;

  const std::string& getName() const noexcept;
  size_t getTableNumber() const noexcept;
  size_t getTimeInMinutes() const noexcept;

  void setName(const std::string& newName);
  void setTableNumber(size_t newTableNum) noexcept;
  void setTimeInMinutes(size_t newTime) noexcept;
private:
  std::string name;
  size_t tableNum;
  size_t timeInMin;
};

Client::Client() :
  name("Unknown"),
  tableNum(0),
  timeInMin(0)
{}

inline Client::Client(std::string clientName, size_t tableNumber, size_t timeMinutes)
  : name(clientName),
  tableNum(tableNumber),
  timeInMin(timeMinutes)
{
  if (clientName.empty())
  {
    name = "Unknown";
  }
}

const std::string& Client::getName() const noexcept
{
  return name;
}

size_t Client::getTableNumber() const noexcept
{
  return tableNum;
}

size_t Client::getTimeInMinutes() const noexcept
{
  return timeInMin;
}

void Client::setName(const std::string& newName)
{
  if (!newName.empty())
  {
    name = newName;
  }
}

void Client::setTableNumber(size_t newTableNum) noexcept
{
  tableNum = newTableNum;
}

void Client::setTimeInMinutes(size_t newTime) noexcept
{
  timeInMin = newTime;
}

#endif
