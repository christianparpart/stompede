#include <stompede/Channel.h>
#include <stompede/Subscriber.h>
#include <stompede/Message.h>
#include <algorithm>

namespace stompede {

void Channel::subscribe(Subscriber* subscriber)
{
	subscribers_.push_back(subscriber);
}

void Channel::unsubscribe(Subscriber* subscriber)
{
	auto i = std::find(subscribers_.begin(), subscribers_.end(), subscriber);
	if (i != subscribers_.end()) {
		subscribers_.erase(i);
	}
}

void Channel::send(Message* message)
{
	for (auto subscriber: subscribers_) {
		subscriber->onMessage(message);
	}
}

} // namespace stompede
