#ifndef CLUB_HPP
#define CLUB_HPP
#include <iostream>
#include <vector>
#include "place.hpp"
#include "DataProcessing.hpp"

class Club
{
public:
  Club();
  Club(const std::string& openTime, const std::string& closeTime, size_t numOfPlaces);
  Club(const Club& other) noexcept = default;
  Club(Club&& other) noexcept = default;
  ~Club() = default;

  const std::string& getOpenTime() const noexcept;
  const std::string& getCloseTime() const noexcept;
  size_t getNumOfPlaces() const noexcept;
  const std::vector< Place >& getPlacesVec() const noexcept;

  bool isInWorkTime(std::string time);

private:
  std::string openTime;
  std::string closeTime;
  size_t numOfPlaces;
  std::vector< Place > places;
};

Club::Club() :
  openTime("00:00"),
  closeTime("23:59"),
  numOfPlaces(0)
{}

Club::Club(const std::string& openTime, const std::string& closeTime, size_t numOfPlaces):
  openTime(openTime),
  closeTime(closeTime),
  numOfPlaces(numOfPlaces)
{
  if (!isValidTimeFormat(openTime) || !isValidTimeFormat(closeTime))
  {
    throw std::invalid_argument("Invalid time format. Expected HH:MM");
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
