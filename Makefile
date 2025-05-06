CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

SRCS := main.cpp DataProcessing.cpp Event.cpp
OBJS := $(SRCS:.cpp=.o)
HEADERS := Club.hpp Place.hpp DataProcessing.hpp Event.hpp

TARGET := club_simulator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
