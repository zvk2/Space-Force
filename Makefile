# all will make all the things
# ALL src comes from /src/, all compiled programs go to /bin/
# run with 'run' on windows, or './run' on unix-like systems
# valid makes:
#	'make' : makes all
#	'make clean' : deletes all files specified by OUT and OBJ
#	'make mrclean' : deletes all files with *.o or *.exe extensions, as well as OUT
#	'make rebuild' : runs clean, and then makes all
#	'make os' : handy little thing, outputs the current OS for troubleshooting purposes
# goal is run "make" in root and it spits out an executable in bin,
# and works regardless of OS with conditionals checking $(OS) and uname

# $< == first dependency, $^ == all dependencies, $@ == target

# declaring paths for source files
OUT = bin/SpaceForce
SRC = $(wildcard src/*.c src/*.cpp) 
DEP = $(wildcard src/*.h src/*.hpp)
OBJ := $(patsubst src/%.cpp, obj/%.o, $(SRC)) 
OBJ := $(patsubst src/%.c, obj/%.o, $(OBJ))
#OBJ = $(src:.cpp=.o) saw this syntax somewhere, threw it in for reference purposes

# set appropriate flags for machine OS. include paths are specified, but do not
# appear to cause problems if all includes are thrown into the base MinGW install.
# this is true even if the specified directories do not exist. 

ifeq ($(OS), Windows_NT)
	DETECTED_OS = $(OS)
	CPP = g++
	CFLAGS = -c -IC:/mingwdev/include/SDL2 -IC:/mingwdev/opengl/include
	INCLUDE = -IC:/mingwdev/include/SDL2 -IC:/mingwdev/opengl/include
	LFLAGS = -LC:/mingwdev/lib -lmingw32 -LC:/mingwdev/opengl/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lopengl32 -lglew32 -lfreeglut -o $(OUT)
else ifeq ($(shell uname -s), Darwin)
	DETECTED_OS := $(shell uname -s)
	CPP = g++ -std=c11
	CFLAGS = -c -I/
	INCLUDE = -I/
	LFLAGS = -o $(OUT) 
else
	DETECTED_OS := $(shell uname -s)
	CPP = clang++
	CFLAGS = -c -I/usr/include/SDL2
	INCLUDE = -I/usr/include/SDL2
	LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -lglut -lGL -lGLU -lm -o $(OUT)
endif

.PHONY: all clean mrclean os rebuild

all: $(OUT)

# target : dependencies
# 	recipe
$(OUT): $(OBJ)
	$(CPP) $^ $(LFLAGS) 

# compile source files to /obj/ (no linking)
# if a header changes, src will recompile
obj/%.o: src/%.c $(DEP)
	$(CPP) $< $(CFLAGS) -o $@

obj/%.o: src/%.cpp $(DEP)
	$(CPP) $< $(CFLAGS) -o $@



# additional features, small tests, etc.
clean:
	rm -f $(OBJ) $(OUT) $(OUT).exe

mrclean:
	rm -f obj/*.o bin/*.exe $(OUT)

rebuild: clean all

os:
	@echo $(DETECTED_OS)
