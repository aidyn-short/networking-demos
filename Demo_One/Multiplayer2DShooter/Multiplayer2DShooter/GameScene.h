#pragma once
#include "Scene.h"

class GameScene: public Scene
{
public:
	GameScene();
	~GameScene();

private:


	
	void Update(float deltaTime) override;

	void Render(SDL_Renderer* renderer) override;

	void HandleEvent(SDL_Event& event) override;


	// Inherited via Scene
	void Init(SDL_Renderer* renderer, SceneManager* manager) override;

};

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}



void GameScene::Update(float deltaTime)
{
}

void GameScene::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

}

void GameScene::HandleEvent(SDL_Event& event)
{
}

void GameScene::Init(SDL_Renderer* renderer, SceneManager* manager)
{
}
