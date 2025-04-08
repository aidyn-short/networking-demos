#pragma once

#include "Scene.h"
#include "SDL2/SDL.h"
#include "Texture.h"
#include "Button.h"
#include "Text.h"
#include "GameScene.h"

Texture menuBackground;
Text* titleText = NULL;
Button* startButton = NULL;
Button* exitButton = NULL;
SDL_Rect titleBackground = { 300, 200, 200, 200 };




class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();


private:

	// Inherited via Scene
	void Init(SDL_Renderer* renderer, SceneManager* manager) override;

	
	void Update(float deltaTime) override;

	void Render(SDL_Renderer* renderer) override;

	void HandleEvent(SDL_Event& event) override;


	SceneManager* manager = NULL;



};

MenuScene::MenuScene()
{


}

MenuScene::~MenuScene()
{
}



void MenuScene::Init(SDL_Renderer* renderer, SceneManager* manager)
{

	this->manager = manager;

	titleText = new Text(renderer,"lazy.ttf", 50,SDL_Color{0,255,0,255}, "Multiplayer Shooter Demo");
	titleText->SetBackgroundColor(SDL_Color{ 0,0,0,255 });
	titleText->SetDimensions(titleText->getTextWidth() + 100, titleText->getTextHeight() + 50);
	titleText->setPosition(SCREEN_WIDTH / 2 - titleText->getTextWidth() / 2, 200);

	menuBackground.loadFromFile(renderer, "menu.png");

	startButton = new Button(renderer, "lazy.ttf", 30, SDL_Color{ 0,255,0,255 }, "Play", [manager,renderer]() {
		GameScene* game = new GameScene();
		manager->ChangeScene(game, renderer);
		});
	startButton->SetDimensions(startButton->getTextWidth() + 50, startButton->getTextHeight() + 10);
	startButton->setPosition(SCREEN_WIDTH / 2 - startButton->getTextWidth() /2, SCREEN_HEIGHT / 2 + 100);
	

	exitButton = new Button(renderer, "lazy.ttf", 30, SDL_Color{ 0,255,0,255 }, "Quit", []() { SDL_Quit();});
	exitButton->SetDimensions(exitButton->getTextWidth() + 50, exitButton->getTextHeight() + 10);
	exitButton->setPosition(SCREEN_WIDTH / 2 - exitButton->getTextWidth() / 2, SCREEN_HEIGHT / 2 + 150);


}

void MenuScene::Update(float deltaTime)
{
}

void MenuScene::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);


	menuBackground.render(renderer, 0, 0);
	startButton->render(renderer);
	exitButton->render(renderer);
	
	titleText->render(renderer);

	//titleText.render(renderer, SCREEN_WIDTH / 2 - titleText.getWidth() / 2, 100);

}

void MenuScene::HandleEvent(SDL_Event& event)
{
	startButton->handleEvent(&event);
	exitButton->handleEvent(&event);
}

