#pragma once

#include <stompede/Api.h>

namespace stompede {

class Message;

class STOMPEDE_API Subscriber
{
public:
	virtual ~Subscriber();

	virtual void onMessage(Message* message) = 0;
};

} // namespace stompede
