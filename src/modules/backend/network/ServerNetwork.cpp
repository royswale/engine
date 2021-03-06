/**
 * @file
 */

#include "ClientMessages_generated.h"
#include "ServerNetwork.h"
#include "core/Trace.h"
#include "core/Log.h"

namespace network {

ServerNetwork::ServerNetwork(const ProtocolHandlerRegistryPtr& protocolHandlerRegistry,
		const core::EventBusPtr& eventBus, const metric::MetricPtr& metric) :
		Super(protocolHandlerRegistry, eventBus, metric) {
}

bool ServerNetwork::packetReceived(ENetEvent& event) {
	flatbuffers::Verifier v(event.packet->data, event.packet->dataLength);

	if (!VerifyClientMessageBuffer(v)) {
		Log::error("Illegal client packet received with length: %i", (int)event.packet->dataLength);
		return false;
	}
	const ClientMessage *req = GetClientMessage(event.packet->data);
	ClientMsgType type = req->data_type();
	const char *clientMsgType = EnumNameClientMsgType(type);
	ProtocolHandlerPtr handler = _protocolHandlerRegistry->getHandler(type);
	if (!handler) {
		Log::error("No handler for client msg type %s", clientMsgType);
		return false;
	}
	const metric::TagMap& tags  {{"direction", "in"}, {"type", clientMsgType}};
	_metric->count("network_packet_count", 1, tags);
	_metric->count("network_packet_size", (int)event.packet->dataLength, tags);

	Log::debug("Received %s", clientMsgType);
	handler->executeWithRaw(event.peer, req->data(), (const uint8_t*)event.packet->data, event.packet->dataLength);
	return true;
}

}
