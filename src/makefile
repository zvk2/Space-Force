# change these two to match local library and header locations
LPATH = -LC:/mingwdev/opengl/lib
INCLUDE = -IC:/mingwdev/opengl/include

CC = g++
LIBS = $(LPATH) -lfreeglut -lGLEW32 -lglu32 -lOpenGL32
SRC =  $(wildcard *.c)
#SRC := $(wildcard ../universal/*.c)
SRC := $(filter-out circle.c, $(SRC))
DEP =  $(wildcard *.h)
#DEP := $(wildcard ../universal/*.h)
OBJ = $(patsubst %.c, %.o, $(SRC))
OUT = stex

# basic rule for compiling all .c into .o into executable
.PHONY: all
all: $(OUT)

$(OUT) : $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(OUT)

%.o: %.c
	$(CC) -c $< $(INCLUDE) $(LIBS) -o $@


## extra stuff
.PHONY: clean rebuild mrclean
clean:
	rm -f $(OBJ) $(OUT) $(OUT).exe

rebuild: clean all

mrclean:
	rm -f *.o *.exe $(OUT)

#-lXi -lXmu
