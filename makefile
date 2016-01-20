CC=gcc
CXX=g++
CFLAGS=-g -Wall -Wextra -std=gnu99
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
	extern/fov.c

CXX_SRC=$(filter %.cpp,$(SRC))
C_SRC=$(filter %.c,$(SRC))
OBJ:=$(C_SRC:.c=.o) $(CXX_SRC:.cpp=.o)

wasteland: $(SRC) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o wasteland
	
clean:
	- rm -f $(OBJ) wasteland
