#pragma once
#include <SDL2/SDL.h>
#include "Scene.h"

class SceneManager
{

private:
	Scene* currentScene = nullptr;

public:
	SceneManager() {}
	~SceneManager() {}

	void ChangeScene(Scene* newScene) {
		if (currentScene) delete currentScene;
		currentScene = newScene;
		currentScene->Init();
	}

	void Update(float deltaTime) { if (currentScene) currentScene->Update(deltaTime); }
	void Render(SDL_Renderer* renderer) { if (currentScene) currentScene->Render(renderer); }
	void HandleEvent(SDL_Event& event) { if (currentScene) currentScene->HandleEvent(event); }


};




