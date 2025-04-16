#pragma once
#include <SDL2/SDL.h>


class Time
{
public:
	static Time& Get() {
		static Time instance;
		return instance;
	}


	void Update() {
		lastDeltaTime = GetDeltaTime();
		deltaTime = SDL_GetTicks() - lastFrameTime;

		lastFrameTime = SDL_GetTicks();

	}


	float GetDeltaTime() { return deltaTime/1000; };

	float deltaTime;
	float lastDeltaTime;
	float lastFrameTime = 0;

private:

};

