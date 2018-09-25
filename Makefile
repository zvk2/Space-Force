# credits makes credits, all will make all the things eventually.
# ALL src comes from /src/, all compiled programs go to /bin/
# run with winrun on windows, or unixrun on unix-like systems
# valid runs:
#	'make' : makes all
#	'make credits' : compiles just credits.cpp, can probably be removed eventually
#	'make os' : handy little thing, outputs the current OS for troubleshooting purposes
# goal is run "make" in root and it spits out an executable in bin,
# and works regardless of OS with conditionals checking $(OS)

# $< == first dependency, $^ == all dependencies, $@ == target

# declaring paths for source files
OUT = bin/SpaceForce
SRC = $(wildcard src/*.cpp)
DEP = $(wildcard src/*.h)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))

# set appropriate flags for windows. will likely need more work for unix-like systems
# once I know more about team member dev environments. include paths are specified,
# but do not appear to cause problems if all includes are thrown into the base MinGW
# install. this is true even if the specified directories do not exist. with luck,
# it's the same in unix.
ifeq ($(OS), Windows_NT)
	DETECTED_OS = $(OS)
	CC = g++
	CFLAGS = -c -IC:/mingwdev/include/SDL2
	INCLUDE = -IC:/mingwdev/include/SDL2
	LFLAGS = -LC:/mingwdev/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o $(OUT)
	LFLAGScr = -LC:/mingwdev/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
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