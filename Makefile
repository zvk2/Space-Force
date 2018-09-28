# THIS WILL BE A CONFLICT FROM conflict_test2

# $< == first dependency, $^ == all dependencies, $@ == target

# declaring paths for source files
OUT = bin/SpaceForce
SRC = $(wildcard src/*.cpp)
DEP = $(wildcard src/*.h)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
#OBJ = $(src:.c=.o) saw this syntax somewhere, threw it in for reference purposes

# set appropriate flags for windows. will likely need more work for unix-like systems
# once I know more about team member dev environments. include paths are specified,
# but do not appear to cause problems if all includes are thrown into the base MinGW
# install. this is true even if the specified directories do not exist. with luck,
# it's the same in unix. 'twould mean our installs can be different and still coexist.

ifeq ($(OS), Windows_NT)
	DETECTED_OS = $(OS)
	CC = g++
	CFLAGS = -c -IC:/mingwdev/include/SDL2
	INCLUDE = -IC:/mingwdev/include/SDL2
	LFLAGS = -LC:/mingwdev/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o $(OUT)
	LFLAGScr = -LC:/mingwdev/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
else ifeq ($(shell uname -s), Darwin)
	DETECTED_OS := $(shell uname -s)
	CC = g++ -std=c11
	CFLAGS = -c -I/
	INCLUDE = -I/
	LFLAGS = -o $(OUT) 
	#LFLAGScr =
else
	DETECTED_OS := $(shell uname -s)
	CC = clang++
	CFLAGS = -c -I/usr/include/SDL2
	INCLUDE = -I/usr/include/SDL2
	LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -o $(OUT)
	LFLAGScr = -lSDL2 -lSDL2_image -lSDL2_ttf
endif

.PHONY: all #credits

all: $(OUT) #credits

# target : dependencies
# 	recipe
$(OUT): $(OBJ)
	$(CC) $^ $(LFLAGS) 

# compile source files to /obj/ (no linking)
# if a header changes, src will recompile
obj/%.o: src/%.cpp $(DEP)
	$(CC) $< $(CFLAGS) -o $@

# additional features for small tests
credits: src/credits.cpp
	$(CC) $< $(INCLUDE) $(LFLAGScr) -o bin/credits

os:
	@echo $(DETECTED_OS)
