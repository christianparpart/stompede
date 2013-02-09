#pragma once

#include <stompede/Api.h>
#include <stompede/Buffer.h>
#include <cstdint>

namespace stompede {
namespace stomp {

enum class Command {
	INVALID,

	// server commands
	SEND,
	SUBSCRIBE,
	UNSUBSCRIBE,
	BEGIN,
	COMMIT,
	ABORT,
	ACK,
	NACK,
	CONNECT,
	DISCONNECT,
	STOMP,

	// client commands
	CONNECTED,
	MESSAGE,
	RECEIPT,
	ERROR
};

} // namespace stomp
} // namespace stompede
