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
		RemoveDisabled();
	}

	void HandleEvent(SDL_Event& event) {
		for (auto& obj : objects) {
			obj->HandleEvent(event);
		}
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

