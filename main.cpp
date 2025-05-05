#include <iostream>
#include <fstream>
#include "club.hpp"
#include "client.hpp"
#include "place.hpp"
#include "DataProcessing.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: ./club_tracker < input_file >" << std::endl;
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile)
  {
    std::cerr << "Failed to open file." << std::endl;
    return 1;
  }
  return 0;
}
