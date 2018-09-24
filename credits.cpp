// This file iterates over every listed file in the folder "Credit_Image" and displays it for 5 seconds
// This uses "sdl0_hello.cpp" as boilerplate

// Includes
#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

// Used for file walk (somewhat crudely)
#include <stdio.h>
#include <dirent.h>

// Constants for resolution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Parent folder for credit images
char CREDITS_FOLDER[] = "Credit_Image/";

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;

bool init() {
	// Flag what subsystems to initialize
	// For now, just video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	
	gWindow = SDL_CreateWindow(
		"Space Force Credits",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	/* Create a renderer for our window
	 * Use hardware acceleration (last arg)
	 * Choose first driver that can provide hardware acceleration
	 *   (second arg, -1)
	 */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr) {	
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			
	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}
	
	return true;
}

SDL_Texture* loadImage(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

void close() {
	for (auto i : gTex) {
		SDL_DestroyTexture(i);
		i = nullptr;
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	// Load media by iterating over the files in "Credit_Image"	
	struct dirent *entry;
	DIR *dp;

	dp = opendir(CREDITS_FOLDER);
	if (dp == NULL) {
		perror("opendir: Path does not exist or could not be read.");
		return -1;
	}

	int images = 0;
	while ((entry = readdir(dp))) {
		int d_name_length = strlen(entry->d_name);
		
		// Crude
		if (d_name_length > 4 && entry->d_name[d_name_length - 4] == '.') {
			// Crude
			char current_image_buffer[2000];
			strcpy(current_image_buffer, CREDITS_FOLDER);
			strcat(current_image_buffer, entry->d_name);
			
			// Puts the image on the buffer queue
			gTex.push_back(loadImage(current_image_buffer));
			images++;
		}
	}
	
	// Close the directory
	closedir(dp);
	
	for (int x=0; x < images; x++) {
		// Clear
		SDL_RenderClear(gRenderer);
		// Render the image
		SDL_RenderCopy(gRenderer, gTex[x], NULL, NULL);
		// Display rendering
		SDL_RenderPresent(gRenderer);
		// Wait 5 seconds
		SDL_Delay(5000);
	}

	// Clear the renderer one last time
	SDL_RenderClear(gRenderer);

	close();
}