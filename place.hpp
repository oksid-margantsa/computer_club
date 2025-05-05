#ifndef PLACE_HPP
#define PLACE_HPP

#include <cstddef>

class Place
{
public:
  Place();
  Place(size_t number);
  Place(const Place& other) noexcept = default;
  Place(Place&& other) noexcept = default;
  ~Place() = default;

  size_t getNumber() const noexcept;
  size_t getBusyTime() const noexcept;
  bool isBusy() const noexcept;

  void occupy(size_t timeMinutes);
  void release();

private:
  size_t numberOfPlace;
  size_t busyTimeInMinutes;
  bool busy;
};

Place::Place():
  numberOfPlace(0),
  busyTimeInMinutes(0),
  busy(false)
{}

Place::Place(size_t number):
  numberOfPlace(number),
  busyTimeInMinutes(0),
  busy(false)
{}

size_t Place::getNumber() const noexcept
{
  return numberOfPlace;
}

size_t Place::getBusyTime() const noexcept
{
  return busyTimeInMinutes;
}

bool Place::isBusy() const noexcept
{
  return busy;
}

void Place::occupy(size_t timeMinutes)
{
  if (!busy && timeMinutes > 0)
  {
    busy = true;
    busyTimeInMinutes += (timeMinutes / 60) * 60 + ((timeMinutes%60 != 0)? 60 : 0);
  }
}

void Place::release()
{
  busy = false;
}

#endif