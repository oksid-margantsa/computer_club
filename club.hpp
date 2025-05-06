#ifndef CLUB_HPP
#define CLUB_HPP
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include  <algorithm>
#include "place.hpp"
#include "DataProcessing.hpp"
#include "event.hpp"

class Club
{
public:
  Club();
  Club(size_t numTables, const std::string& openTime, const std::string& closeTime, size_t pricePerHour);
  Club(const Club& other) noexcept = default;
  Club(Club&& other) noexcept = default;
  ~Club() = default;

  const std::string& getOpenTime() const noexcept;
  const std::string& getCloseTime() const noexcept;
  size_t getNumOfPlaces() const noexcept;
  const std::vector< Place >& getPlacesVec() const noexcept;

  bool isInWorkTime(std::string time);
  void processEvent(const Event& event);
  void endOfDay();
  void printResults() const;

private:
  std::string openTime;
  std::string closeTime;
  size_t numOfPlaces;
  size_t pricePerHour;

  std::vector< Place > places;
  std::queue< std::string > waitingQueue;
  std::set< std::string > clientsInClub;
  std::map< std::string, size_t > clientToTable;
  std::set< std::string > outputEvents;

  void handleClientArrived(const Event& event);
  void handleClientSat(const Event& event);
  void handleClientWaiting(const Event& event);
  void handleClientLeft(const Event& event);
  void addErrorEvent(const std::string& time, const std::string& error);
  void addOutputEvent(const Event& event);
  void seatClientFromQueue(size_t tableNumber);
};

Club::Club() :
  numOfPlaces(0),
  pricePerHour(0)
{}

Club::Club(size_t numTables, const std::string& openTime, const std::string& closeTime, size_t pricePerHour) :
  openTime(openTime),
  closeTime(closeTime),
  numOfPlaces(numTables),
  pricePerHour(pricePerHour)
{
  places.reserve(numTables);
  for (size_t i = 1; i <= numTables; ++i)
  {
    places.emplace_back(i);
  }
}

void Club::processEvent(const Event& event)
{
  addOutputEvent(event);

  switch (event.getType())
  {
  case EventType::CLIENT_ARRIVED:
    handleClientArrived(event);
    break;
  case EventType::CLIENT_SAT:
    handleClientSat(event);
    break;
  case EventType::CLIENT_WAITING:
    handleClientWaiting(event);
    break;
  case EventType::CLIENT_LEFT:
    handleClientLeft(event);
    break;
  default:
    break;
  }
}

void Club::handleClientArrived(const Event& event)
{
  const auto& clientName = event.getArgs()[0];

  if (!isInWorkTime(event.getTime()))
  {
    addErrorEvent(event.getTime(), "NotOpenYet");
    return;
  }

  if (clientsInClub.find(clientName) != clientsInClub.end())
  {
    addErrorEvent(event.getTime(), "YouShallNotPass");
    return;
  }

  clientsInClub.insert(clientName);
}

void Club::handleClientSat(const Event& event)
{
  const auto& clientName = event.getArgs()[0];
  size_t tableNumber = std::stoul(event.getArgs()[1]);
  if (clientsInClub.find(clientName) == clientsInClub.end())
  {
    addErrorEvent(event.getTime(), "ClientUnknown");
    return;
  }
  if (tableNumber < 1 || tableNumber > numOfPlaces)
  {
    return;
  }
  if (places[tableNumber - 1].isBusy())
  {
    addErrorEvent(event.getTime(), "PlaceIsBusy");
    return;
  }
  auto it = clientToTable.find(clientName);
  if (it != clientToTable.end())
  {
    places[it->second - 1].release();
  }
  places[tableNumber - 1].occupy(timeFormatToMinutes(closeTime) - timeFormatToMinutes(event.getTime()));
  clientToTable[clientName] = tableNumber;
}

void Club::handleClientWaiting(const Event& event)
{
  const auto& clientName = event.getArgs()[0];

  if (clientsInClub.find(clientName) == clientsInClub.end())
  {
    addErrorEvent(event.getTime(), "ClientUnknown");
    return;
  }

  for (const auto& place : places)
  {
    if (!place.isBusy())
    {
      addErrorEvent(event.getTime(), "ICanWaitNoLonger!");
      return;
    }
  }
  if (waitingQueue.size() >= numOfPlaces)
  {
    clientsInClub.erase(clientName);
    addOutputEvent(Event(event.getTime(), EventType::CLIENT_FORCED_LEFT, { clientName }));
    return;
  }
  waitingQueue.push(clientName);
}

void Club::handleClientLeft(const Event& event)
{
  const auto& clientName = event.getArgs()[0];
  if (clientsInClub.find(clientName) == clientsInClub.end())
  {
    addErrorEvent(event.getTime(), "ClientUnknown");
    return;
  }
  clientsInClub.erase(clientName);
  auto it = clientToTable.find(clientName);
  if (it != clientToTable.end())
  {
    size_t tableNumber = it->second;
    clientToTable.erase(it);
    places[tableNumber - 1].release();
    seatClientFromQueue(tableNumber);
  }
}

void Club::seatClientFromQueue(size_t tableNumber)
{
  if (!waitingQueue.empty())
  {
    std::string clientName = waitingQueue.front();
    waitingQueue.pop();

    places[tableNumber - 1].occupy(timeFormatToMinutes(closeTime) - timeFormatToMinutes(minutesInTimeFormat(places[tableNumber - 1].getBusyTime())));
    clientToTable[clientName] = tableNumber;

    addOutputEvent(Event(minutesInTimeFormat(timeFormatToMinutes(closeTime) - places[tableNumber - 1].getBusyTime()),
      EventType::CLIENT_SAT_FROM_QUEUE, { clientName, std::to_string(tableNumber) }));
  }
}

void Club::addErrorEvent(const std::string& time, const std::string& error)
{
  addOutputEvent(Event(time, EventType::ERROR, { error }));
}

void Club::addOutputEvent(const Event& event)
{
  std::string eventStr = event.getTime() + " " + std::to_string(static_cast< int >(event.getType()));
  for (const auto& arg : event.getArgs())
  {
    eventStr += " " + arg;
  }
  outputEvents.insert(eventStr);
}

void Club::endOfDay()
{
  std::vector< std::string > clientsLeft(clientsInClub.begin(), clientsInClub.end());
  std::sort(clientsLeft.begin(), clientsLeft.end());

  for (const auto& client : clientsLeft)
  {
    addOutputEvent(Event(closeTime, EventType::CLIENT_FORCED_LEFT, { client }));

    auto it = clientToTable.find(client);
    if (it != clientToTable.end()) {
      places[it->second - 1].release();
    }
  }

  clientsInClub.clear();
  clientToTable.clear();
  while (!waitingQueue.empty())
  {
    waitingQueue.pop();
  }
}

void Club::printResults() const
{
  std::cout << openTime << std::endl;

  for (const auto& event : outputEvents)
  {
    std::cout << event << std::endl;
  }

  std::cout << closeTime << std::endl;

  for (const auto& place : places)
  {
    size_t hours = place.getBusyTime() / 60;
    size_t minutes = place.getBusyTime() % 60;
    char timeBuf[6];
    std::sprintf(timeBuf, "%02zu:%02zu", hours, minutes);
    std::cout << place.getNumber() << " " << (hours + (minutes > 0 ? 1 : 0)) * pricePerHour << " " << timeBuf << std::endl;
  }
}

inline bool Club::isInWorkTime(std::string time)
{
  size_t openTimeInMinutes = timeFormatToMinutes(openTime);
  size_t closeTimeInMinutes = timeFormatToMinutes(closeTime);
  if (!isValidTimeFormat(time))
  {
    return false;
  }
  size_t timeInMinutes = timeFormatToMinutes(time);
  if ((openTimeInMinutes <= timeInMinutes) and (timeInMinutes <= closeTimeInMinutes))
  {
    return true;
  }
  else
  {
    return false;
  }
}

const std::string& Club::getOpenTime() const noexcept
{
  return openTime;
}

const std::string& Club::getCloseTime() const noexcept
{
  return closeTime;
}

size_t Club::getNumOfPlaces() const noexcept
{
  return numOfPlaces;
}

const std::vector< Place >& Club::getPlacesVec() const noexcept
{
  return places;
}

#endif
