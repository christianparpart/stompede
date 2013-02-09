#include <stompede/Domain.h>
#include <stompede/Channel.h>

namespace stompede {

Domain::Domain() :
	channels_()
{
}

Domain::~Domain()
{
	for (auto channel: channels_)
		delete channel.second;
}

bool Domain::contains(const std::string& destination) const
{
	return find(destination) != nullptr;
}

Channel* Domain::find(const std::string& destination) const
{
	auto ci = channels_.find(destination);
	if (likely(ci != channels_.end()))
		return ci->second;

	return nullptr;
}

Channel* Domain::findOrCreate(const std::string& destination)
{
	auto ci = channels_.find(destination);
	if (likely(ci != channels_.end()))
		return ci->second;

	//! \todo support different kind of channels (pubsub, fifo, ... more?)

	/*
	 * Destination prefixes:
	 *
	 * "/queue/" ::= implements a FIFO queue
	 * "/topic/" ::= implements a pubsub channel
	 *
	 * In ActiveMQ, STOMP-prefixes are stripped off before passing them to JMS,
	 * so we might want to strip off before even reaching here, in the stomp layer,
	 * and then tweak the channel's type by some attribute (Routing)
	 */

	Channel* channel = new Channel(this, RoutingPolicy::Everyone);
	channels_[destination] = channel;
	return channel;
}

} // namespace stompede
