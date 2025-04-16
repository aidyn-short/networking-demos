#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wall.h"




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



	Player* playerOne = NULL;
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Texture* background = NULL;
	Wall* wallOne = NULL;

};

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(SDL_Renderer* renderer, SceneManager* manager)
{
	Texture* playerTexture = new Texture();
	TTF_Font* font = TTF_OpenFont("lazy.ttf", 20);
	playerTexture->loadFromRenderedText(renderer, "i", SDL_Color{ 255,255,255,255 }, font);
	background = new Texture();
	background->loadFromFile(renderer, "menu.png");

	playerOne = new Player(*playerTexture);





	wallOne = new Wall(renderer, 400, 400, 0, "left.bmp");



}



void GameScene::Update(float deltaTime)
{



	GameObjectRegistry::Get().UpdateAll(deltaTime);



}

void GameScene::Render(SDL_Renderer* renderer)
{
	


	camera.x = (playerOne->getPosX() + Player::Player_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (playerOne->getPosY() + Player::Player_HEIGHT / 2) - SCREEN_HEIGHT / 2;


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

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
	

	background->render(renderer,-100 - camera.x,-100 - camera.y);

	wallOne->Render(renderer, SDL_Point{ camera.x, camera.y });
	playerOne->Render(renderer, SDL_Point{camera.x, camera.y});





}

void GameScene::HandleEvent(SDL_Event& event)
{
	GameObjectRegistry::Get().HandleEvent(event);

}

