CC=gcc
CXX=g++
CFLAGS=-g -Wall -Wextra -std=gnu99 -fsanitize=address
CXXFLAGS=-g -Wall -Wextra --std=gnu++14 -Iextern -fsanitize=address
LDFLAGS=-fsanitize=address

.POSIX:

SFML_LDFLAGS:=-lsfml-system -lsfml-window -lsfml-graphics
LDFLAGS += $(SFML_LDFLAGS)

default: .POSIX wasteland

SRC:=\
	wasteland.cpp\
	character.cpp\
	map.cpp\
	utility.cpp\
	object.cpp\
	levelgen.cpp\
	extern/fov.c

CXX_SRC=$(filter %.cpp,$(SRC))
C_SRC=$(filter %.c,$(SRC))
OBJ:=$(C_SRC:.c=.o) $(CXX_SRC:.cpp=.o)

wasteland: $(SRC) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o wasteland
	
clean:
	- rm -f $(OBJ) wasteland
