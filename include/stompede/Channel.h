#pragma once

#include <stompede/Api.h>
#include <string>
#include <utility>
#include <unordered_map>
#include <list>

namespace stompede {

class Message;
class Subscriber;

class STOMPEDE_API Channel
{
public:
	void subscribe(Subscriber* subscriber);
	void unsubscribe(Subscriber* subscriber);

	void send(Message* message);

private:
	std::list<Subscriber*> subscribers_;
};

} // namespace stompede
