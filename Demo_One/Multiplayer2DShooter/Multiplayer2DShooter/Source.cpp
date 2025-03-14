#define SDL_MAIN_HANDLED
#include  <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "LTexture.h"
#include <cmath>

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



SDL_Rect gSpriteClips[4];
LTexture gSpriteSheetTexture;

//Angle of rotation
double degrees = 0;

//Flip type
SDL_RendererFlip flipType = SDL_FLIP_NONE;

TTF_Font* gFont = NULL;

LTexture gTextTexture;



//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};


bool init() {

	bool success = true;


	

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Couldn't initalize";
		success = false;

	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}


		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Window couldn't be created";
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				std::cout << "Failed to create renderer";
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);



				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "Failed to init pngs";
					success = false;
				}

				if (TTF_Init() == -1)
				{
					std::cout << "Could not initalize SDL_TTF";
					success = false;
				}


			}

			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}

	}


	return success;
}


bool loadMedia() {

	bool success = true;


	gFont = TTF_OpenFont("lazy.ttf", 28);
	if (gFont == NULL)
	{
		std::cout << "Failed to load font";
		success = false;
	}
	else
	{
		SDL_Color textColor = { 0,0,0,0 };
		if (!gTextTexture.loadFromRenderedText(gRenderer, "The quick brown fox jumps over the lazy dog", textColor, gFont))
		{
			std::cout << "Failed to load text texture";
			success = false;
		}
	}



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

	if (!gSpriteSheetTexture.loadFromFile(gRenderer, "dots.png"))
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else
	{
		//Set top left sprite
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 100;
		gSpriteClips[0].h = 100;

		//Set top right sprite
		gSpriteClips[1].x = 100;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 100;
		gSpriteClips[1].h = 100;

		//Set bottom left sprite
		gSpriteClips[2].x = 0;
		gSpriteClips[2].y = 100;
		gSpriteClips[2].w = 100;
		gSpriteClips[2].h = 100;

		//Set bottom right sprite
		gSpriteClips[3].x = 100;
		gSpriteClips[3].y = 100;
		gSpriteClips[3].w = 100;
		gSpriteClips[3].h = 100;
	}




	return success;


}


void close() {

	gFooTexture.free();
	gBackgroundTexture.free();

	SDL_FreeSurface(gCurrentSurface);
	gCurrentSurface = NULL;
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}






int main(int argc, char* args[])
{

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
					if (e.type == SDL_QUIT)
					{
						quit = true;
						std::cout << "QUIT";
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_a:
							degrees -= 60;
							break;

						case SDLK_d:
							degrees += 60;
							break;

						case SDLK_q:
							flipType = SDL_FLIP_HORIZONTAL;
							break;

						case SDLK_w:
							flipType = SDL_FLIP_NONE;
							break;

						case SDLK_e:
							flipType = SDL_FLIP_VERTICAL;
							break;
						}
					}

				
			
					//gBackgroundTexture.SetColor(100, 200, 0);


					//gBackgroundTexture.setBlendMode(SDL_BLENDMODE_BLEND);

				//	gBackgroundTexture.setAlpha(100);

					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

				//	gBackgroundTexture.render(gRenderer,0, 0);


					gTextTexture.render(gRenderer, (SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);

					//gFooTexture.render(gRenderer,240, 190, NULL, degrees, NULL, flipType);

					//Render top left sprite
				//	gSpriteSheetTexture.render(gRenderer,0, 0, &gSpriteClips[0]);

					//Render top right sprite
				//	gSpriteSheetTexture.render(gRenderer, SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);

					//Render bottom left sprite
				//	gSpriteSheetTexture.render(gRenderer, 0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);

					//Render bottom right sprite
				//	gSpriteSheetTexture.render(gRenderer, SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);


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
		}

	}

		close();
	
	

	return 0;
}