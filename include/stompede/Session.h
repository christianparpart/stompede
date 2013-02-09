#pragma once

#include <stompede/Api.h>
#include <string>

namespace stompede {

class Message;

class STOMPEDE_API Session
{
public:
	Session();
	virtual ~Session();

	const std::string& id() const { return id_; }

	virtual void receiveMessage(Message* message) = 0;

private:
	std::string id_;
};

} // namespace stompede
