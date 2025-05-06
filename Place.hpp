#ifndef PLACE_HPP
#define PLACE_HPP

#include <string>
#include "DataProcessing.hpp"

class Place
{
public:
  Place(size_t number, size_t pricePerHour);

  bool isBusy() const;
  void occupy(const std::string& time);
  void release(const std::string& time);
  size_t getRevenue() const;
  std::string getOccupiedTime() const;
  size_t getNumber() const;

private:
  size_t number;
  size_t pricePerHour;
  bool busy;
  std::string occupiedSince;
  size_t totalMinutes;
  size_t revenue;
};

Place::Place(size_t number, size_t pricePerHour) :
  number(number),
  pricePerHour(pricePerHour),
  busy(false),
  totalMinutes(0),
  revenue(0)
{
}

bool Place::isBusy() const
{
  return busy;
}

void Place::occupy(const std::string& time)
{
  if (!busy)
  {
    busy = true;
    occupiedSince = time;
  }
}

void Place::release(const std::string& time)
{
  if (busy)
  {
    size_t start = timeFormatToMinutes(occupiedSince);
    size_t end = timeFormatToMinutes(time);
    size_t duration = end - start;

    totalMinutes += duration;
    size_t hours = duration / 60;
    if (duration % 60 != 0) hours++;
    revenue += hours * pricePerHour;

    busy = false;
  }
}

size_t Place::getRevenue() const
{
  return revenue;
}

std::string Place::getOccupiedTime() const
{
  return minutesInTimeFormat(totalMinutes);
}

size_t Place::getNumber() const
{
  return number;
}

#endif