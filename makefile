CXX=g++
CXXFLAGS=-g -Wall -Wextra --std=gnu++14
LDFLAGS=

.POSIX:

SFML_LDFLAGS:=-lsfml-system -lsfml-window -lsfml-graphics
CXXFLAGS += $(SFML_LDFLAGS)

default: wasteland

SRC:=\
	wasteland.cpp\
	character.cpp\
	map.cpp\
	utility.cpp\
	object.cpp\

OBJ:=$(SRC:.cpp=.o)

wasteland: $(SRC) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o wasteland
	
clean:
	- rm -f $(OBJ) wasteland
