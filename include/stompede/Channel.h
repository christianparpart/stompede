#pragma once

#include <stompede/Api.h>
#include <string>
#include <utility>
#include <list>

namespace stompede {

class Domain;
class Message;
class Session;

//! Channel message specification.
enum class RoutingPolicy {
	//! The message gets delivered to exactly one subscriber in that channel (like a FIFO pipe channel).
	ExactlyOne,

	//! The messages gets delivered to everyone subscribed in that channel (classic pubsub channel).
	Everyone,
};

/*! A channel instance, used for end-to-end communication.
 *
 * \see Domain, Session, Message
 */
class STOMPEDE_API Channel
{
public:
	Channel(Domain* owner, RoutingPolicy routingPolicy);
	~Channel();

	RoutingPolicy routingPolicy() const { return routingPolicy_; }
	void setRoutingPolicy(RoutingPolicy policy) { routingPolicy_ = policy; }

	void subscribe(Session* subscriber);
	void unsubscribe(Session* subscriber);

	void send(Message* message);

private:
	Domain* domain_;
	RoutingPolicy routingPolicy_;
	std::list<Session*> subscribers_;
};

} // namespace stompede
