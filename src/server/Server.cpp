/**
 * @file
 */

#include "Server.h"

#include "io/Filesystem.h"
#include "core/Var.h"
#include "core/command/Command.h"
#include "cooldown/CooldownProvider.h"
#include "network/ServerNetwork.h"
#include "network/ServerMessageSender.h"
#include "attrib/ContainerProvider.h"
#include "poi/PoiProvider.h"
#include "eventmgr/EventMgr.h"
#include "backend/entity/EntityStorage.h"
#include "backend/entity/ai/AIRegistry.h"
#include "backend/entity/ai/AILoader.h"
#include "backend/loop/ServerLoop.h"
#include "backend/spawn/SpawnMgr.h"
#include "backend/world/MapProvider.h"
#include "persistence/DBHandler.h"
#include "stock/StockDataProvider.h"
#include "metric/UDPMetricSender.h"
#include <cstdlib>

Server::Server(const backend::ServerLoopPtr& serverLoop,
		const core::TimeProviderPtr& timeProvider, const io::FilesystemPtr& filesystem,
		const core::EventBusPtr& eventBus) :
		Super(filesystem, eventBus, timeProvider, 15678),
		_serverLoop(serverLoop) {
	_syslog = true;
	_coredump = true;
	init(ORGANISATION, "server");
}

core::AppState Server::onConstruct() {
	const core::AppState state = Super::onConstruct();

	core::Var::get(cfg::MetricFlavor, "telegraf");
	core::Var::get(cfg::MetricHost, "127.0.0.1");
	core::Var::get(cfg::MetricPort, "8125");
	core::Var::get(cfg::DatabaseName, "engine");
	core::Var::get(cfg::DatabaseHost, "localhost");
	core::Var::get(cfg::DatabaseUser, "engine");
	core::Var::get(cfg::DatabasePassword, "engine", core::CV_SECRET);
	core::Var::get(cfg::ServerUserTimeout, "60000");
	core::Var::get(cfg::ServerPort, "11337");
	core::Var::get(cfg::ServerHost, "");
	core::Var::get(cfg::ServerMaxClients, "1024");
	core::Var::get(cfg::ServerSeed, "1");
	core::Var::get(cfg::VoxelMeshSize, "16", core::CV_READONLY);
	core::Var::get(cfg::DatabaseMinConnections, "2");
	core::Var::get(cfg::DatabaseMaxConnections, "10");

	return state;
}

core::AppState Server::onInit() {
	const core::AppState state = Super::onInit();
	if (state != core::AppState::Running) {
		return state;
	}

	if (!_serverLoop->init()) {
		Log::error("Failed to init the main loop");
		return core::AppState::Cleanup;
	}

	return core::AppState::Running;
}

core::AppState Server::onCleanup() {
	const core::AppState state = Super::onCleanup();
	_serverLoop->shutdown();
	return state;
}

core::AppState Server::onRunning() {
	Super::onRunning();
	_serverLoop->update(_deltaFrame);
	return core::AppState::Running;
}

int main(int argc, char *argv[]) {
	const core::EventBusPtr& eventBus = std::make_shared<core::EventBus>();
	const core::TimeProviderPtr& timeProvider = std::make_shared<core::TimeProvider>();
	const io::FilesystemPtr& filesystem = std::make_shared<io::Filesystem>();
	const backend::AIRegistryPtr& registry = std::make_shared<backend::AIRegistry>();
	const attrib::ContainerProviderPtr& containerProvider = std::make_shared<attrib::ContainerProvider>();

	const network::ProtocolHandlerRegistryPtr& protocolHandlerRegistry = std::make_shared<network::ProtocolHandlerRegistry>();
	const network::ServerNetworkPtr& network = std::make_shared<network::ServerNetwork>(protocolHandlerRegistry, eventBus);
	const network::ServerMessageSenderPtr& messageSender = std::make_shared<network::ServerMessageSender>(network);

	const backend::AILoaderPtr& loader = std::make_shared<backend::AILoader>(registry);

	const cooldown::CooldownProviderPtr& cooldownProvider = std::make_shared<cooldown::CooldownProvider>();

	const stock::StockProviderPtr& stockDataProvider = std::make_shared<stock::StockDataProvider>();
	const persistence::DBHandlerPtr& dbHandler = std::make_shared<persistence::DBHandler>();
	const poi::PoiProviderPtr& poiProvider = std::make_shared<poi::PoiProvider>(timeProvider);
	const backend::MapProviderPtr& mapProvider = std::make_shared<backend::MapProvider>(filesystem, eventBus);
	const backend::EntityStoragePtr& entityStorage = std::make_shared<backend::EntityStorage>(mapProvider, messageSender,
			timeProvider, containerProvider, poiProvider, cooldownProvider, dbHandler, stockDataProvider, eventBus);

	const eventmgr::EventProviderPtr& eventProvider = std::make_shared<eventmgr::EventProvider>(dbHandler);
	const eventmgr::EventMgrPtr& eventMgr = std::make_shared<eventmgr::EventMgr>(eventProvider, timeProvider);

	const metric::IMetricSenderPtr& metricSender = std::make_shared<metric::UDPMetricSender>();
	const backend::SpawnMgrPtr& spawnMgr = std::make_shared<backend::SpawnMgr>(filesystem, entityStorage, messageSender,
			timeProvider, loader, containerProvider, poiProvider, cooldownProvider);
	const backend::WorldPtr& world = std::make_shared<backend::World>(mapProvider, spawnMgr, registry, eventBus, filesystem);
	const backend::ServerLoopPtr& serverLoop = std::make_shared<backend::ServerLoop>(world, dbHandler, network,
			filesystem, entityStorage, eventBus, containerProvider, poiProvider, cooldownProvider,
			eventMgr, stockDataProvider, metricSender);

	Server app(serverLoop, timeProvider, filesystem, eventBus);
	return app.startMainLoop(argc, argv);
}
