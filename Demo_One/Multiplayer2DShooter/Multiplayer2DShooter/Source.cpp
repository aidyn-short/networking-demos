#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include<SDL3_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "LTexture.h"

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gCurrentSurface = NULL;

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;

SDL_Surface* gPNGSurface = NULL;



enum KeyPressSurfaces 
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_W,
	KEY_PRESS_SURFACE_S,
	KEY_PRESS_SURFACE_A,
	KEY_PRESS_SURFACE_D,
	KEY_PRESS_SURFACE_TOTAL
};


SDL_Surface* gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL];

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

LTexture gFooTexture;

LTexture gBackgroundTexture;

bool init() {

	bool success = true;


	

	if (!SDL_Init(SDL_INIT_VIDEO))
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
			gRenderer = SDL_CreateRenderer(gWindow, NULL);
			if (gRenderer == NULL)
			{
				std::cout << "Failed to create renderer";
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}

			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}

	}


	return success;
}


SDL_Texture* loadTexture(std::string path) {

	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load img";
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture from loadedSurface";
		}
		SDL_DestroySurface(loadedSurface);

	}

	return newTexture;

}



SDL_Surface* loadSurface(std::string path) {

	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* temportarySurface =  IMG_Load(path.c_str());

	if (temportarySurface == NULL)
	{
		std::cout << "Unable to load image";
		return NULL;
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(temportarySurface, gScreenSurface->format);
		if (optimizedSurface == NULL)
		{
			std::cout << "Optimized image failed";
		}
		SDL_DestroySurface(temportarySurface);
	}

	return optimizedSurface;


}

bool loadMedia() {

	bool success = true;

	if (!gFooTexture.loadFromFile(gRenderer, "foo.bmp"))
	{
		std::cout << "Failed to load foo img";
		success = false;
	}

	if (!gBackgroundTexture.loadFromFile(gRenderer, "background.bmp"))
	{
		std::cout << "failed to load background";
		success = false;
	}
/*
	gTexture = loadTexture("texture.bmp");
	if (gTexture == NULL)
	{
		std::cout << "Failed to load texture from image";
		success = false;
	}
	*/


	/*
	gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("press.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		std::cout << "Unable to load image";
		success = false;
	}


	gKeyPressSurface[KEY_PRESS_SURFACE_W] = loadSurface("up.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_W] == NULL)
	{
		printf("Failed to load up image!\n");
		success = false;
	}

	//Load down surface
	gKeyPressSurface[KEY_PRESS_SURFACE_S] = loadSurface("down.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_S] == NULL)
	{
		printf("Failed to load down image!\n");
		success = false;
	}

	//Load left surface
	gKeyPressSurface[KEY_PRESS_SURFACE_A] = loadSurface("left.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_A] == NULL)
	{
		printf("Failed to load left image!\n");
		success = false;
	}

	//Load right surface
	gKeyPressSurface[KEY_PRESS_SURFACE_D] = loadSurface("right.bmp");
	if (gKeyPressSurface[KEY_PRESS_SURFACE_D] == NULL)
	{
		printf("Failed to load right image!\n");
		success = false;
	}

	*/




	return success;


}


void close() {

	gFooTexture.free();
	gBackgroundTexture.free();

	SDL_DestroySurface(gCurrentSurface);
	gCurrentSurface = NULL;
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;



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




			bool quit = false;
			SDL_Event e;
			//gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
			
			


			while (!quit)
			{

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_EVENT_QUIT)
					{
						quit = true;
						std::cout << "QUIT";
					}


					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
			

					gBackgroundTexture.render(gRenderer,0, 0);

					gFooTexture.render(gRenderer,240, 190);
	/*
					SDL_FRect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
					SDL_RenderFillRect(gRenderer, &fillRect);
					*/
				//	SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);

					SDL_RenderPresent(gRenderer);

					/*
					switch (e.key.key)
					{
					case SDLK_W:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_W];
						break;

					case SDLK_S:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_S];
						break;

					case SDLK_A:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_A];
						break;

					case SDLK_D:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_D];
						break;

					default:
						gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
						break;
					}

					SDL_Rect stretchRect;
					stretchRect.x = 0;
					stretchRect.y = 0;
					stretchRect.w = SCREEN_WIDTH;
					stretchRect.h = SCREEN_HEIGHT;

					SDL_BlitSurfaceScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect, SDL_SCALEMODE_LINEAR);

					//SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
					SDL_UpdateWindowSurface(gWindow);
					*/
					
				}
			}

			
			//SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_EVENT_QUIT) quit = true; } }
		}

	}

		close();
	
	

	return 0;
}