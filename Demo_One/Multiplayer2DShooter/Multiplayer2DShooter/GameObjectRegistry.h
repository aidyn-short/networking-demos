#pragma once
#include "GameObject.h"
#include <vector>
#include <unordered_map>I 
#include <sstream>
#include "player.h"
#include "Rifle.h"
#include "RifleProjectile.h"



class GameObjectRegistry
{
public:
	static GameObjectRegistry& Get() {
		static GameObjectRegistry instance;
		return instance;
	}


	void SetClient(int clientNumber) { this->clientNumber = clientNumber; std::cout << clientNumber << std::endl << std::endl; }

	int GetClientNumber() { return clientNumber; }


	void Add(GameObject* object) {
		pendingObjects.push_back(object);

		objectMap[std::to_string(object->GetClientID()) + std::to_string(object->GetID())] = object;

	}

	void AddPending() {
		if (!pendingObjects.empty()) {
			objects.insert(objects.end(),pendingObjects.begin(), pendingObjects.end());
			pendingObjects.clear();
		}

	}


	GameObject* GetByID(std::string id) {
		auto object = objectMap.find(id);
		if (object != objectMap.end())
		{
			return object->second;
		}
		return nullptr;
	}



	const std::vector<GameObject*> getAll() {
		return objects;
	}

	
	void UpdateAll(float dt) {
		
		for (auto& obj : objects) {
			if (obj->GetClientID() == clientNumber)
			{
				obj->Update(dt);
			}
			
		}

	}


	void RenderAll(SDL_Renderer* renderer, SDL_Point camera) {

		for (auto& obj : objects) {
			if (obj->GetEnabled())
			{
				obj->Render(renderer, camera);

			}
			
		}
	}


	std::string TrimMessage(const std::string& raw) {
		size_t start = raw.find_first_not_of(" \"");
		size_t end = raw.find_last_not_of(" \"");
		return (start == std::string::npos || end == std::string::npos)
			? ""
			: raw.substr(start, end - start + 1);
	}


	std::string Write() {

		std::string messageToSend;
		for (auto& obj : objects) {
			if (obj->GetClientID() == clientNumber)
			{
				std::string potentialMsg = obj->Write();

				if (potentialMsg != "")
				{
					potentialMsg += "+";
					messageToSend += potentialMsg;
				}


			
			}

		}
		return messageToSend;

	}



	void UpdateNetworked(std::string udpMessage) {

		size_t start = udpMessage.find("[");
		size_t end = udpMessage.find("]");
		std::string messages_block = udpMessage.substr(start + 1, end - start - 1);
		

		messages_block.erase(std::remove(messages_block.begin(), messages_block.end(), '\"'), messages_block.end());

		
		std::vector<std::string> udpMessages;
		std::stringstream ss(messages_block);
		std::string item;

		while (std::getline(ss, item, '+')) {
			udpMessages.push_back(TrimMessage(item));
		}

		for (const std::string& msg : udpMessages) {

			std::vector<std::string> parts;
			std::stringstream msgStream(msg);
			std::string segment;

			while (std::getline(msgStream, segment, '=')) {
				parts.push_back(segment);
			}

			// Optional: remove trailing '/' from last part
			if (!parts.empty() && parts.back().back() == '/') {
				parts.back().pop_back();
			}

			if (parts.size() < 2)
			{
				continue;
			}

			if (parts[1] ==std::to_string(clientNumber))
			{
				// do nothing it's a local object that's already updated
				//std::cout << "local";
			}
			else if (GetByID((parts[1] + parts[2])) == nullptr)
			{
				//Create new object

				if (parts[0] == "player")
				{
					Player* remotePlayer = new Player();
					remotePlayer->Read(parts);
					Add(remotePlayer);
			
				}
				if (parts[0] == "rifleProjectile")
				{
					RifleProjectile* remoteProjectile = new RifleProjectile(0,0,0);
					remoteProjectile->Read(parts);
					Add(remoteProjectile);
			

				}
			}
			else
			{
				GetByID((parts[1] + parts[2]))->Read(parts);
		
			}
		}
	}



	void HandleEvent(SDL_Event& event) {
		for (auto& obj : objects) {
			if (obj->GetClientID() == clientNumber)
			{
				obj->HandleEvent(event);
			}
		}
		AddPending();
	}


	void CheckCollision() {
		for (size_t i = 0; i < objects.size(); i++)
		{
			for (size_t j = i + 1; j < objects.size(); ++j) {

				GameObject& objectA = *objects[i];
				GameObject& objectB = *objects[j];

				if (objectA.objectType == "wall" && objectB.objectType == "wall")
				{
					continue;
				}
				else if (IsColliding(objectA.collision, objectA.angle * (3.1415/180), objectB.collision, objectB.angle * (3.1415 / 180)))
				{
					objectA.HandleCollision(&objectB);
					objectB.HandleCollision(&objectA);
				}

			}
		}
	}



	bool IsColliding(SDL_FRect& rectA, double rotationA, SDL_FRect& rectB, double rotationB) {
		SDL_Point centerA = { rectA.x + rectA.w / 2, rectA.y + rectA.h / 2 };
		SDL_Point centerB = { rectB.x + rectB.w / 2, rectB.y + rectB.h / 2 };

		// Get rotated corners of the rectangles
		auto cornersA = GetRotatedCorners(rectA, centerA, rotationA);
		auto cornersB = GetRotatedCorners(rectB, centerB, rotationB);

		// Get edges for both rectangles and check for overlap
		std::vector<SDL_Point> axes;

		// Get the edge normals for rectA
		for (size_t i = 0; i < cornersA.size(); ++i) {
			SDL_Point edge = { cornersA[(i + 1) % cornersA.size()].x - cornersA[i].x,
							  cornersA[(i + 1) % cornersA.size()].y - cornersA[i].y };
			axes.push_back({ -edge.y, edge.x }); // Normal to the edge
		}

		// Get the edge normals for rectB
		for (size_t i = 0; i < cornersB.size(); ++i) {
			SDL_Point edge = { cornersB[(i + 1) % cornersB.size()].x - cornersB[i].x,
							  cornersB[(i + 1) % cornersB.size()].y - cornersB[i].y };
			axes.push_back({ -edge.y, edge.x }); // Normal to the edge
		}

		// Check for overlap on each axis
		for (auto& axis : axes) {
			if (!OverlapOnAxis(cornersA, cornersB, axis)) {
				return false; // No collision if no overlap on any axis
			}
		}

		return true; // Collision if there is overlap on all axes
	}

	bool OverlapOnAxis(std::vector<SDL_Point>& rect1, std::vector<SDL_Point>& rect2, SDL_Point axis) {
		// Normalize axis
		float length = std::sqrt(axis.x * axis.x + axis.y * axis.y);
		if (length == 0) return true; // Avoid divide-by-zero
		float nx = axis.x / length;
		float ny = axis.y / length;

		float min1 = std::numeric_limits<float>::infinity();
		float max1 = -std::numeric_limits<float>::infinity();
		for (const auto& p : rect1) {
			float projection = p.x * nx + p.y * ny;
			min1 = std::min(min1, projection);
			max1 = std::max(max1, projection);
		}

		float min2 = std::numeric_limits<float>::infinity();
		float max2 = -std::numeric_limits<float>::infinity();
		for (const auto& p : rect2) {
			float projection = p.x * nx + p.y * ny;
			min2 = std::min(min2, projection);
			max2 = std::max(max2, projection);
		}

		return !(max1 < min2 || max2 < min1);
	}



	// Rotates a point around a center by an angle in radians
	SDL_Point RotatePoint(SDL_Point p, SDL_Point center, double angle) {
		double s = sin(angle);
		double c = cos(angle);

		p.x -= center.x;
		p.y -= center.y;

		double newX = p.x * c - p.y * s;
		double newY = p.x * s + p.y * c;

		p.x = static_cast<int>(newX + center.x);
		p.y = static_cast<int>(newY + center.y);

		return p;
	}


	std::vector<SDL_Point> GetRotatedCorners(SDL_FRect& rect, SDL_Point center, double angle) {
		std::vector<SDL_Point> corners;

		SDL_Point topLeft = { rect.x, rect.y };
		SDL_Point topRight = { rect.x + rect.w, rect.y };
		SDL_Point bottomRight = { rect.x + rect.w, rect.y + rect.h };
		SDL_Point bottomLeft = { rect.x, rect.y + rect.h };

		corners.push_back(RotatePoint(topLeft, center, angle));
		corners.push_back(RotatePoint(topRight, center, angle));
		corners.push_back(RotatePoint(bottomRight, center, angle));
		corners.push_back(RotatePoint(bottomLeft, center, angle));

		return corners;
	}



	void RemoveDisabled() {
		auto it = objects.begin();
		while (it != objects.end()) {
			if (!(*it)->GetEnabled()) {
				GameObject* obj = *it;
				if (obj->deleteObject)
				{
					delete obj;
				}
				it = objects.erase(it);

			}
			else {
				++it;
			}
		}
	}


private:

	std::vector<GameObject*> pendingObjects;

	std::vector<GameObject*> objects;
	std::unordered_map<std::string, GameObject*> objectMap; 

	
	static bool IsEnabled(GameObject* object) {
		return !object->GetEnabled();
	}

	



	
	int clientNumber = 0;

};

