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


const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int SCREEN_HEIGHT = 1080/2;
const int SCREEN_WIDTH = 1920/2;


SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


SDL_Renderer* gRenderer = NULL;


LTexture gBackgroundTexture;




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

		
		}

	}


	return success;
}





void close() {




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
							
						
					}
				

					playerOne.handleEvent(e);

				}

				playerOne.move();


				camera.x = (playerOne.getPosX() + Player::Player_WIDTH / 2) - SCREEN_WIDTH / 2;
				camera.y = (playerOne.getPosY() + Player::Player_HEIGHT / 2) - SCREEN_HEIGHT / 2;

				if (camera.x < 0)
				{
					camera.x = 0;
				}
				if (camera.y < 0)
				{
					camera.y = 0;
				}
				if (camera.x > LEVEL_WIDTH - camera.w)
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if (camera.y > LEVEL_HEIGHT - camera.h)
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

				timeText.str("");
				timeText << "Seconds since start time " << avgFPS;

				buttonOne.SetText(gRenderer, timeText.str().c_str());

				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
				SDL_RenderClear(gRenderer);


				gBackgroundTexture.render(gRenderer, 0, 0, &camera);
				
				SDL_Point cameraPos = { camera.x, camera.y };
				//buttonOne.render(gRenderer);
				playerOne.render(gRenderer,cameraPos);
				SDL_RenderPresent(gRenderer);
			}
		

	}

		close();
	
	

	return 0;
}