/**
 * @file
 */

#pragma once

#include "Map.h"
#include "backend/ForwardDecl.h"
#include <unordered_map>

namespace backend {

class World {
private:
	SpawnMgrPtr _spawnMgr;
	MapProviderPtr _mapProvider;
	AIRegistryPtr _registry;
	core::EventBusPtr _eventBus;
	ai::Server* _aiServer = nullptr;
	std::unordered_map<MapId, MapPtr> _maps;
public:
	World(const MapProviderPtr& mapProvider, const SpawnMgrPtr& spawnMgr, const AIRegistryPtr& registry,
			const core::EventBusPtr& eventBus);

	void update(long dt);

	MapPtr map(MapId id) const;

	bool init();
	void shutdown();
};

inline MapPtr World::map(MapId id) const {
	auto i = _maps.find(id);
	if (i == _maps.end()) {
		return MapPtr();
	}
	return i->second;
}

}
