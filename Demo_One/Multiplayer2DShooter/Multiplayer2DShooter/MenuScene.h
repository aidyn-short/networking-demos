#pragma once

#include "Scene.h"
#include "SDL2/SDL.h"
#include "Texture.h"
#include "Button.h"


Texture menuBackground;
Texture titleText;
Button* startButton = NULL;
Button* exitButton = NULL;





class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

private:


	// Inherited via Scene
	void Init(SDL_Renderer* renderer) override;

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

void MenuScene::Init(SDL_Renderer* renderer)
{


	TTF_Font* font = TTF_OpenFont("lazy.ttf", 20);
	titleText.loadFromRenderedText(renderer, "Multiplayer Shooter Demo", SDL_Color{255,255,255,255}, font);
	
	


}

void MenuScene::Update(float deltaTime)
{
}

void MenuScene::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0xFF);
	SDL_RenderClear(renderer);

	titleText.render(renderer, SCREEN_WIDTH / 2 - titleText.getWidth() / 2, SCREEN_HEIGHT / 2 - titleText.getHeight() / 2);




}

void MenuScene::HandleEvent(SDL_Event& event)
{
}
