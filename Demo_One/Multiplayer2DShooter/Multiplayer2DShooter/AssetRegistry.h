#pragma once
#include <unordered_map>
#include <string>
#include <typeindex>
#include <type_traits>
#include "Texture.h"




class AssetRegistry
{
public:

	bool objectRemoved = false;

	static AssetRegistry& Get() {
		static AssetRegistry instance;
		return instance;
	}

	template<typename T>
	void Register(const std::string& name, T* asset) {
		auto& map = GetRegistry<T>();
		map[name] = asset;
	}

	template<typename T>
	T* GetAsset(const std::string& name) {
		auto& map = GetRegistry<T>();
		auto it = map.find(name);
		if (it != map.end()) {
			return it->second;
		}
		return nullptr;
	}

private:

	AssetRegistry() = default;

	template<typename T>
	std::unordered_map<std::string, T*>& GetRegistry() {
		static std::unordered_map<std::string, T*> map;
		return map;
	}
};
