#pragma once
#include "Scene.h"

class GameScene: public Scene
{
public:
	GameScene();
	~GameScene();

private:


	// Inherited via Scene
	void Init() override;

	void Update(float deltaTime) override;

	void Render(SDL_Renderer* renderer) override;

	void HandleEvent(SDL_Event& event) override;

};

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
}

void GameScene::Update(float deltaTime)
{
}

void GameScene::Render(SDL_Renderer* renderer)
{
}

void GameScene::HandleEvent(SDL_Event& event)
{
}
