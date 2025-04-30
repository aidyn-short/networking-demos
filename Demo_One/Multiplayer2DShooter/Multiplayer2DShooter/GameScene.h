#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wall.h"
#include "Socket.h"
#include "Rifle.h"
#include <vector>





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
	std::vector<Texture*> background;




	Wall* wallOne = NULL;
	Socket* tcpSocket = NULL;
	Socket* udpSocket = NULL;
	

	Rifle* rifleOne = NULL;


};

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

}

void GameScene::Init(SDL_Renderer* renderer, SceneManager* manager)
{

	tcpSocket = new Socket(Socket::Family::INET, Socket::Type::TCP);
	tcpSocket->Connect(Address("127.0.0.1", 5555));

	char recvMsg[4096];



	int msgSize =  tcpSocket->Recv(recvMsg, 4096);


	std::string clientNumber(recvMsg, msgSize);


	tcpSocket->SetNonBlockingMode(true);

	GameObjectRegistry::Get().SetClient(std::stoi(clientNumber));



	udpSocket = new Socket(Socket::Family::INET, Socket::Type::UDP);
	udpSocket->SetNonBlockingMode(true);
	


	for (int x = 0; x < 30; x++)
	{
		for (int y = 0; y < 30; y++)
		{
			Texture* newTexture = AssetRegistry::Get().GetAsset<Texture>("background");
			background.push_back(newTexture);
		}

	}


	for (int i = 0; i < 30; i++)
	{
		Wall* topWall = new Wall(renderer, i * 100, 0, 0 , "wall.bmp");
		Wall* leftWall = new Wall(renderer, 0, i * 100, 0, "wall.bmp");

		Wall* rightall = new Wall(renderer, LEVEL_WIDTH - 100, i * 100, 0, "wall.bmp");

		Wall* bottomWall = new Wall(renderer, i * 100, LEVEL_HEIGHT - 100, 0, "wall.bmp");


	}

	std::srand(10);


	for (int i = 0; i < 100; i++)
	{
		Wall* randomWall = new Wall(renderer, 100* (std::rand() % 21 + 2), 100 * (std::rand() % 21 + 2), 0, "wall.bmp");

	}

	for (int i = 0; i < 20; i++)
	{
		Rifle* randomRifle = new Rifle(100 * (std::rand() % 21 + 2 + .5f), 100 * (std::rand() % 21 + 2 + .5f));

	}



	Texture* playerTexture = new Texture();
	TTF_Font* font = TTF_OpenFont("lazy.ttf", 20);
	playerTexture->loadFromRenderedText(renderer, "i", SDL_Color{ 255,255,255,255 }, font);

	playerOne = new Player();
	playerOne->SetClient(std::stoi(clientNumber));


	std::srand(std::time(0));
	SDL_Point playerSpawn = playerSpawns[std::rand() % 4 + 1];
	playerOne->posX = playerSpawn.x;
	playerOne->posY = playerSpawn.y;
	GameObjectRegistry::Get().Add(playerOne);


	rifleOne = new Rifle(400, 200);


	


	wallOne = new Wall(renderer, 400, 400, 0, "wall.bmp");



}





void GameScene::Update(float deltaTime)
{



	GameObjectRegistry::Get().UpdateAll(deltaTime);


	char buffer[4096];

	Address address = Address("127.0.0.1", 5556);

	std::string message = GameObjectRegistry::Get().Write();


	udpSocket->SendTo(message.data(), message.size(), Address("127.0.0.1", 5556));

	size_t recvSize =  udpSocket->RecvFrom(buffer, 4096, address);



	if (recvSize > 0 && recvSize < 4096)
	{
		std::string recvMsg(buffer, recvSize);



		GameObjectRegistry::Get().UpdateNetworked(recvMsg);
		
	}

	
	GameObjectRegistry::Get().CheckCollision();
	GameObjectRegistry::Get().RemoveDisabled();


	


	


}

void GameScene::Render(SDL_Renderer* renderer)
{
	


	camera.x = (playerOne->getPosX() + Player::Player_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (playerOne->getPosY() + Player::Player_HEIGHT / 2) - SCREEN_HEIGHT / 2;


	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
	



	for (int x = 0; x < 30; x++)
	{
		for (int y = 0; y < 30; y++)
		{
			background[x * 30 + y]->render(renderer, x * 100 - camera.x, y * 100 - camera.y);
		}

	}

	

	GameObjectRegistry::Get().RenderAll(renderer, SDL_Point{ camera.x, camera.y });


	

	//wallOne->Render(renderer, SDL_Point{ camera.x, camera.y });
	//playerOne->Render(renderer, SDL_Point{camera.x, camera.y});





}

void GameScene::HandleEvent(SDL_Event& event)
{
	GameObjectRegistry::Get().HandleEvent(event);

}

