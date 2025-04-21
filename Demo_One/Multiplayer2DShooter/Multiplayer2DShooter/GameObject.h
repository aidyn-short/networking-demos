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


	virtual void Update(float deltaTime) {}
	virtual void Render(SDL_Renderer* renderer, SDL_Point camPos) {}
	virtual void HandleEvent(SDL_Event& event) {}
	virtual bool HandleCollision(GameObject* collidingObject) { return false; }
	virtual std::string Write() { return ""; }
	virtual void Read(std::vector<std::string> info) {}

	void SetClient(int clientNumber) { client = clientNumber; }

	bool GetEnabled() const { return enabled; }

	int GetID() { return id; }
	int GetClientID() {return client;}
	void SetID(int newID) { id = newID; }

	void SetEnalbed(bool enabled) { this->enabled = enabled;  }

	SDL_FRect collision;
	Texture texture;
	float posX = 0, posY = 0;
	float angle = 0.000;
	std::string objectType;
	int client = 0;
	bool deleteObject = false;




private:
	int id;
	bool enabled = true;
	static int generateUniqueId() {
		static int currentId = 0;
		return currentId++;
	}

};


