#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Club.hpp"
#include "Place.hpp"
#include "DataProcessing.hpp"

int main(int argc, char* argv[]) {
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile)
  {
    std::cerr << "Failed to open file: " << argv[1] << std::endl;
    return 1;
  }

  try
  {
    std::string line;
    size_t lineNumber = 0;

    if (!std::getline(infile, line))
    {
      std::cerr << "Error: Empty input file" << std::endl;
      return 1;
    }
    lineNumber++;

    size_t numTables;
    try
    {
      numTables = std::stoul(line);
    }
    catch (...)
    {
      std::cerr << "Error at line " << lineNumber << ": Invalid number of tables" << std::endl;
      return 1;
    }

    if (numTables == 0)
    {
      std::cerr << "Error at line " << lineNumber << ": Number of tables must be positive" << std::endl;
      return 1;
    }

    if (!std::getline(infile, line))
    {
      std::cerr << "Error at line " << lineNumber + 1 << ": Missing working hours" << std::endl;
      return 1;
    }
    lineNumber++;

    line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) { return !std::isspace(ch); }));
    line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) { return !std::isspace(ch); }).base(), line.end());

    auto hours = split(line, ' ');
    if (hours.size() != 2)
    {
      std::cerr << "Error at line " << lineNumber << ": Invalid working hours format. Expected exactly two times separated by space" << std::endl;
      return 1;
    }

    std::string openTime = hours[0];
    std::string closeTime = hours[1];

    if (!isValidTimeFormat(openTime) || !isValidTimeFormat(closeTime))
    {
      std::cerr << "Error at line " << lineNumber << ": Invalid time format in working hours" << std::endl;
      return 1;
    }

    if (!std::getline(infile, line))
    {
      std::cerr << "Error at line " << lineNumber + 1 << ": Missing price per hour" << std::endl;
      return 1;
    }
    lineNumber++;

    size_t pricePerHour;
    try
    {
      pricePerHour = std::stoul(line);
    }
    catch (...)
    {
      std::cerr << "Error at line " << lineNumber << ": Invalid price per hour" << std::endl;
      return 1;
    }

    if (pricePerHour == 0)
    {
      std::cerr << "Error at line " << lineNumber << ": Price per hour must be positive" << std::endl;
      return 1;
    }

    Club club(numTables, openTime, closeTime, pricePerHour);

    std::string prevTime = "00:00";
    while (std::getline(infile, line))
    {
      lineNumber++;
      if (line.empty()) continue;

      line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) { return !std::isspace(ch); }));
      line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) { return !std::isspace(ch); }).base(), line.end());

      try
      {
        Event event = parseEvent(line);

        if (event.getTime() < prevTime)
        {
          std::cerr << "Error at line " << lineNumber
            << ": Events must be in chronological order" << std::endl;
          return 1;
        }
        prevTime = event.getTime();

        if (!isValidTimeFormat(event.getTime()))
        {
          std::cerr << "Error at line " << lineNumber
            << ": Invalid time format '" << event.getTime() << "'" << std::endl;
          return 1;
        }

        club.processEvent(event);
      }
      catch (const std::exception& e)
      {
        std::cerr << "Error at line " << lineNumber << ": " << e.what() << std::endl;
        std::cerr << "Line content: " << line << std::endl;
        return 1;
      }
    }
    club.endOfDay();
    club.printResults();

  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}