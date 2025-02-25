#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>


SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gImage = NULL;

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;

bool init() {

	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Couldn't initalize";
		success = false;

	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (gWindow == NULL)
		{
			std::cout << "Window couldn't be created";
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}

	}


	return success;
}

bool loadMedia() {

	bool success = true;

	gImage = SDL_LoadBMP("hello_world.bmp");
	if (gImage == NULL)
	{
		std::cout << "Unable to load image";
		success = false;
	}

	return success;


}


void close() {

	SDL_DestroySurface(gImage);
	gImage = NULL;


	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}


int main(int argc, char* argv[]) {

	if (!init())
	{
		std::cout << "Failed to init";
	}
	else 
	{
		if (!loadMedia())
		{
			std::cout << "Failed to load media";
		}
		else
		{
			SDL_BlitSurface(gImage, NULL , gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
			SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_EVENT_QUIT) quit = true; } }
		}

	}

		close();
	
	

	return 0;
}