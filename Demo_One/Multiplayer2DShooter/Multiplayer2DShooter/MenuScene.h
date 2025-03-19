#pragma once

#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

private:


	// Inherited via Scene
	void Init() override;

	void Update(float deltaTime) override;

	void Render(SDL_Renderer* renderer) override;

	void HandleEvent(SDL_Event& event) override;

};

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
}

void MenuScene::Update(float deltaTime)
{
}

void MenuScene::Render(SDL_Renderer* renderer)
{
}

void MenuScene::HandleEvent(SDL_Event& event)
{
}
