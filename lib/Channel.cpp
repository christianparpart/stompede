#include <stompede/Channel.h>
#include <stompede/Session.h>
#include <stompede/Message.h>
#include <algorithm>

namespace stompede {

Channel::Channel(Domain* owner, RoutingPolicy routingPolicy) :
	domain_(owner),
	routingPolicy_(routingPolicy),
	subscribers_()
{
}

Channel::~Channel()
{
	//! \todo hard kill all subscriptions
}

void Channel::subscribe(Session* subscriber)
{
	subscribers_.push_back(subscriber);
}

void Channel::unsubscribe(Session* subscriber)
{
	auto i = std::find(subscribers_.begin(), subscribers_.end(), subscriber);

	if (i != subscribers_.end()) {
		subscribers_.erase(i);
	}
}

void Channel::send(Message* message)
{
	switch (routingPolicy_) {
		case RoutingPolicy::Everyone:
			for (auto subscriber: subscribers_) {
				subscriber->receiveMessage(message);
			}
			break;
		case RoutingPolicy::ExactlyOne:
			//! \todo implement round-robbin, least-load, ...
			break;
	}
}

} // namespace stompede
