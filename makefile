CXX=g++
CXXFLAGS=-g -Wall -Wextra --std=gnu++11
LDFLAGS=

.POSIX:

default: wasteland

SRC:=\
	wasteland.cpp\
	map.cpp\

OBJ:=$(SRC:.cpp=.o)

wasteland: $(SRC) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o wasteland
	
clean:
	- rm -f $(OBJ) wasteland
