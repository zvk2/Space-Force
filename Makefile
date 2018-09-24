# Right now, this MAKE file just makes the credits
credits: credits.cpp
	clang++ -o credits credits.cpp -I/usr/include/SDL2 -lSDL2 -lSDL2_image -lSDL2_ttf
