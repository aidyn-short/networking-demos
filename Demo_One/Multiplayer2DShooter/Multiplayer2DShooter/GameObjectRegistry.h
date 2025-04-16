#pragma once
#include "GameObject.h"
#include <vector>


class GameObjectRegistry
{
public:
	static GameObjectRegistry& Get() {
		static GameObjectRegistry instance;
		return instance;
	}


	void Add(GameObject* object) {
		objects.push_back(object);
	}


	const std::vector<GameObject*> getAll() {
		return objects;
	}

	
	void UpdateAll(float dt) {
		
		for (auto& obj : objects) {
			obj->Update(dt);
		}
		CheckCollision();
		RemoveDisabled();
	
	}

	void HandleEvent(SDL_Event& event) {
		for (auto& obj : objects) {
			obj->HandleEvent(event);
		}
	}


	void CheckCollision() {
		for (size_t i = 0; i < objects.size(); i++)
		{
			for (size_t j = i + 1; j < objects.size(); ++j) {
				GameObject& objectA = *objects[i];
				GameObject& objectB = *objects[j];
				if (IsColliding(objectA.collision, objectA.angle, objectB.collision, objectB.angle))
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

private:

	std::vector<GameObject*> objects;
	
	
	static bool IsEnabled(GameObject* object) {
		return !object->GetEnabled();
	}

	void RemoveDisabled() {
		auto deletedObjects = std::remove_if(objects.begin(), objects.end(), IsEnabled);

		for (auto object = deletedObjects; object != objects.end(); ++object)
		{
			delete* object;
		}

		objects.erase(deletedObjects, objects.end());
	}

	

};

