#pragma once
#include <SDL2/SDL.h>
#include "Time.h"
#include "AssetRegistry.h"
#include <vector>

class GameObject
{
public:


	GameObject() {
		id = generateUniqueId();
	}


	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer, SDL_Point camPos) = 0;
	virtual void HandleEvent(SDL_Event& event) = 0;
	virtual bool HandleCollision(GameObject* collidingObject) = 0;
	virtual std::string Write() { return NULL; }
	virtual void Read(std::vector<std::string> info) {}

	void SetClient(int clientNumber) { client = clientNumber; }

	bool GetEnabled() { return enabled; }

	int GetID() { return id; }
	int GetClientID() {return client;}
	void SetID(int newID) { id = newID; }

	bool enabled = true;

	SDL_FRect collision;
	Texture texture;
	float posX, posY;
	float angle = 0.000;
	std::string objectType;
	int client;
private:
	int id;

	static int generateUniqueId() {
		static int currentId = 0;
		return currentId++;
	}

};


