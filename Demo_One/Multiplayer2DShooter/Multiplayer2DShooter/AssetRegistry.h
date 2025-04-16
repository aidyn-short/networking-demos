#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>
#include <type_traits>

class AssetRegistry
{
public:

	bool objectRemoved = false;


	static AssetRegistry& Get() {
		static AssetRegistry instance;
		return instance;
	}

	template<typename T>
	void Register(const std::string& name, std::shared_ptr<T> asset) {
		static_assert(std::is_base_of<Asset, T>::value || std::is_class<T>::value, "Asset must be a class.");
		auto& map = GetRegistry<T>();
		map[name] = asset;
	}

	template<typename T>
	std::shared_ptr<T> Get(const std::string& name) {
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
	std::unordered_map<std::string, std::shared_ptr<T>>& GetRegistry(){
		static std::unordered_map<std::string, std::shared_ptr<T>> map;
		return map;
	}


};
