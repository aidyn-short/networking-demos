#define SDL_MAIN_HANDLED
#include  <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Texture.h"
#include <cmath>
#include "Button.h"
#include <SDL2/SDL_mixer.h>
#include <sstream>
#include "Player.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "Socket.h"
#include "AssetRegistry.h"



SDL_Window* gWindow = NULL;








SDL_Renderer* renderer = NULL;


Texture gBackgroundTexture;

SceneManager sceneManager;



bool init() {

	SocketInit();


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
			renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				std::cout << "Failed to create renderer";
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);



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






	//Init all the assets


	Texture* playerTexture = new Texture();
	playerTexture->loadFromFile(renderer, "survivor-idle_flashlight_0.png");
	playerTexture->SetScale(.33f, .33f);
	AssetRegistry::Get().Register("player", playerTexture);
	


	Texture* playerRifle = new Texture();
	playerRifle->loadFromFile(renderer, "survivor-shoot_rifle_0.png");
	playerRifle->SetScale(.33f, .33f);
	AssetRegistry::Get().Register("playerRifle", playerRifle);




	Texture* rifleTexture = new Texture();
	rifleTexture->loadFromFile(renderer, "rifle.png");
	AssetRegistry::Get().Register("rifle", rifleTexture);


	Texture* projectileTexture = new Texture();
	projectileTexture->loadFromFile(renderer, "Zombie-Tileset---_0371_Capa-372.png");
	AssetRegistry::Get().Register("rifleProjectile", projectileTexture);


	
	return success;
}





void close() {




	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(renderer);
	gWindow = NULL;
	renderer = NULL;



	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
	SocketShutdown();
}






int main(int argc, char* args[])
{







	if (!init())
	{
		std::cout << "Failed to init";
	}
	else
	{
		MenuScene* menu = new MenuScene();
		sceneManager.ChangeScene(menu, renderer);

		bool quit = false;
		SDL_Event e;
		while (!quit)
		{
			Time::Get().Update();

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
					std::cout << "QUIT";
				}
				
				sceneManager.HandleEvent(e);
			}
			sceneManager.Update(Time::Get().GetDeltaTime());
			sceneManager.Render(renderer);

			SDL_RenderPresent(renderer);
		}

	}
	close();
}



	/*
			SDL_Color textColor = { 255,255,255,255 };
			Button buttonOne(gRenderer, "lazy.ttf", 20, textColor, "WOW I'M a rendered button");
			buttonOne.setPosition(200, 200);

			TTF_Font* font = TTF_OpenFont("lazy.ttf", 20);


			Texture playerTexture;
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
}*/