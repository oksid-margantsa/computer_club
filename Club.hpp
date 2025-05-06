#ifndef CLUB_HPP
#define CLUB_HPP
#include <vector>
#include <iostream>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include "Place.hpp"
#include "DataProcessing.hpp"
#include "Event.hpp"

class Club
{
public:
  Club(size_t numTables, const std::string& openTime, const std::string& closeTime, size_t pricePerHour);
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
  std::vector< std::string > outputEvents;

  bool isInWorkTime(const std::string& time) const;
  void handleClientArrived(const Event& event);
  void handleClientSat(const Event& event);
  void handleClientWaiting(const Event& event);
  void handleClientLeft(const Event& event);
  void addErrorEvent(const std::string& time, const std::string& error);
  void addOutputEvent(const std::string& eventStr);
  void seatClientFromQueue(size_t tableNumber, const std::string& currentTime);
};

Club::Club(size_t numTables, const std::string& openTime, const std::string& closeTime, size_t pricePerHour) :
  openTime(openTime),
  closeTime(closeTime),
  numOfPlaces(numTables),
  pricePerHour(pricePerHour)
{
  for (size_t i = 1; i <= numTables; ++i)
  {
    places.emplace_back(i, pricePerHour);
  }
}

bool Club::isInWorkTime(const std::string& time) const
{
  size_t open = timeFormatToMinutes(openTime);
  size_t close = timeFormatToMinutes(closeTime);
  size_t current = timeFormatToMinutes(time);
  return current >= open && current <= close;
}

void Club::processEvent(const Event& event)
{
  addOutputEvent(event.toString());

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
    places[it->second - 1].release(event.getTime());
  }

  places[tableNumber - 1].occupy(event.getTime());
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
    if (!place.isBusy()) {
      addErrorEvent(event.getTime(), "ICanWaitNoLonger!");
      return;
    }
  }

  if (waitingQueue.size() >= numOfPlaces)
  {
    clientsInClub.erase(clientName);
    addOutputEvent(Event(event.getTime(), EventType::CLIENT_FORCED_LEFT, { clientName }).toString());
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
    places[tableNumber - 1].release(event.getTime());
    clientToTable.erase(it);
    seatClientFromQueue(tableNumber, event.getTime());
  }
}

void Club::seatClientFromQueue(size_t tableNumber, const std::string& currentTime)
{
  if (!waitingQueue.empty())
  {
    std::string clientName = waitingQueue.front();
    waitingQueue.pop();

    places[tableNumber - 1].occupy(currentTime);
    clientToTable[clientName] = tableNumber;

    addOutputEvent(Event(currentTime, EventType::CLIENT_SAT_FROM_QUEUE,
      { clientName, std::to_string(tableNumber) }).toString());
  }
}

void Club::addErrorEvent(const std::string& time, const std::string& error)
{
  addOutputEvent(Event(time, EventType::ERROR, { error }).toString());
}

void Club::addOutputEvent(const std::string& eventStr)
{
  outputEvents.push_back(eventStr);
}

void Club::endOfDay()
{
  std::vector< std::string > clientsLeft(clientsInClub.begin(), clientsInClub.end());
  std::sort(clientsLeft.begin(), clientsLeft.end());

  for (const auto& client : clientsLeft)
  {
    addOutputEvent(Event(closeTime, EventType::CLIENT_FORCED_LEFT, { client }).toString());

    auto it = clientToTable.find(client);
    if (it != clientToTable.end())
    {
      places[it->second - 1].release(closeTime);
    }
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
    std::cout << place.getNumber() << " "
      << place.getRevenue() << " "
      << place.getOccupiedTime() << std::endl;
  }
}

#endif