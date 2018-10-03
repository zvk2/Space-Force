# all will make all the things
# ALL src comes from /src/, all compiled programs go to /bin/
# run with 'run' on windows, or './run' on unix-like systems
# valid makes:
#	'make' : makes all
#	'make os' : handy little thing, outputs the current OS for troubleshooting purposes
# goal is run "make" in root and it spits out an executable in bin,
# and works regardless of OS with conditionals checking $(OS) and uname

# $< == first dependency, $^ == all dependencies, $@ == target

# declaring paths for source files
OUT = bin/SpaceForce
SRC = $(wildcard src/*.cpp)
SRC := $(filter-out src/menu.cpp, $(SRC))
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
else ifeq ($(shell uname -s), Darwin)
	DETECTED_OS := $(shell uname -s)
	CC = g++ -std=c11
	CFLAGS = -c -I/
	INCLUDE = -I/
	LFLAGS = -o $(OUT) 
else
	DETECTED_OS := $(shell uname -s)
	CC = clang++
	CFLAGS = -c -I/usr/include/SDL2
	INCLUDE = -I/usr/include/SDL2
	LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -o $(OUT)
endif

.PHONY: all clean mrclean os rebuild

all: $(OUT)

# target : dependencies
# 	recipe
$(OUT): $(OBJ)
	$(CC) $^ $(LFLAGS) 

# compile source files to /obj/ (no linking)
# if a header changes, src will recompile
obj/%.o: src/%.cpp $(DEP)
	$(CC) $< $(CFLAGS) -o $@

# additional features, small tests, etc.
clean:
	rm -f $(OBJ) $(OUT) $(OUT).exe

mrclean:
	rm -f obj/*.o bin/*.exe $(OUT)

os:
	@echo $(DETECTED_OS)

rebuild: clean all