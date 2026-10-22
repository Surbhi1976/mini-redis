CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -pthread -Iinclude
SRC      = $(wildcard src/*.cpp)

server: $(SRC) $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) $(SRC) -o server

clean:
	rm -f server

.PHONY: clean
