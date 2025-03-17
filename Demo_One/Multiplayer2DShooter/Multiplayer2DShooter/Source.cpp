#define SDL_MAIN_HANDLED
#include  <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "LTexture.h"
#include <cmath>
#include "LButton.h"
#include <SDL2/SDL_mixer.h>
#include "Timer.h"
#include <sstream>
#include "Player.h"

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

//The music that will be played
Mix_Music* gMusic = NULL;

//The sound effects that will be used
Mix_Chunk* gScratch = NULL;
Mix_Chunk* gHigh = NULL;
Mix_Chunk* gMedium = NULL;
Mix_Chunk* gLow = NULL;




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
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					std::cout << "Failed to init mixer";
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


	gMusic = Mix_LoadMUS("beat.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effects
	gScratch = Mix_LoadWAV("scratch.wav");
	if (gScratch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gHigh = Mix_LoadWAV("high.wav");
	if (gHigh == NULL)
	{
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gMedium = Mix_LoadWAV("medium.wav");
	if (gMedium == NULL)
	{
		printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	gLow = Mix_LoadWAV("low.wav");
	if (gLow == NULL)
	{
		printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}


	return success;


}


void close() {

	gFooTexture.free();
	gBackgroundTexture.free();


	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gMedium);
	Mix_FreeChunk(gLow);
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;

	Mix_FreeMusic(gMusic);
	gMusic = NULL;

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
	Mix_Quit();
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
			SDL_Color textColor = { 255,255,255,255 };
			LButton buttonOne(gRenderer, "lazy.ttf", 20, textColor, "WOW I'M a rendered button");
			buttonOne.setPosition(200, 200);

			TTF_Font* font = TTF_OpenFont("lazy.ttf", 20);


			LTexture playerTexture;
			playerTexture.loadFromRenderedText(gRenderer, "i", textColor, font);
			
			Player playerOne(playerTexture);

			bool quit = false;
			SDL_Event e;
			//gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
			
			Timer timer;

			int countedFrames = 0;
			timer.start();

			std::stringstream timeText;


			while (!quit)
			{
				countedFrames++;
				float avgFPS = countedFrames / (timer.getTicks() / 1000.f);
				if (avgFPS > 2000000)
				{
					avgFPS = 0;
				}

				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
						std::cout << "QUIT";
					}
					else if (e.type == SDL_KEYDOWN)
					{


						//Start/stop
						if (e.key.keysym.sym == SDLK_s)
						{
							if (timer.isStarted())
							{
								timer.stop();
							}
							else
							{
								timer.start();
							}
						}
						//Pause/unpause
						else if (e.key.keysym.sym == SDLK_p)
						{
							if (timer.isPaused())
							{
								timer.unpause();
							}
							else
							{
								timer.pause();
							}
						}



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
						case SDLK_1:
							Mix_PlayChannel(-1, gHigh, 0);
							break;
						case SDLK_2:
							Mix_PlayChannel(-1, gMedium, 0);
							break;
						case SDLK_3:
							Mix_PlayChannel(-1, gLow, 0);
							break;
						case SDLK_4:
							Mix_PlayChannel(-1, gScratch, 0);
							break;
						case SDLK_9:
							if (Mix_PlayingMusic() == 0)
							{
								Mix_PlayMusic(gMusic, -1);
							}
							else
							{
								if (Mix_PausedMusic() == 1)
								{
									//Resume the music
									Mix_ResumeMusic();
								}
								//If the music is playing
								else
								{
									//Pause the music
									Mix_PauseMusic();
								}
							}
							break;
						case SDLK_0:
							Mix_HaltMusic();
							Mix_HaltChannel(-1);
							break;
						}
							
						
					}
				
					buttonOne.handleEvent(&e);
					playerOne.handleEvent(e);

				}

				playerOne.move();

				timeText.str("");
				timeText << "Seconds since start time " << avgFPS;

				buttonOne.SetText(gRenderer, timeText.str().c_str());

				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
				SDL_RenderClear(gRenderer);


			
		
				buttonOne.render(gRenderer);
				playerOne.render(gRenderer);
				SDL_RenderPresent(gRenderer);
			}
		}

	}

		close();
	
	

	return 0;
}