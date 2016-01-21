CC=gcc
CXX=g++
CFLAGS=-gdwarf-4 -fvar-tracking-assignments -Wall -Wextra -std=gnu99 -fsanitize=address
CXXFLAGS=-gdwarf-4 -fvar-tracking-assignments -Wall -Wextra --std=gnu++14 -Iextern -fsanitize=address
LDFLAGS=-fsanitize=address

.POSIX:

RAPIDXML_FLAGS:= -Iextern/rapidxml-1.13
SFML_LDFLAGS:=-lsfml-system -lsfml-window -lsfml-graphics

CXXFLAGS += $(RAPIDXML_FLAGS)
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
