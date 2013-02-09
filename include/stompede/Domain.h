#pragma once

#include <stompede/Api.h>
#include <unordered_map>
#include <string>

namespace stompede {

class Channel;

/*! a domain (or namespace) of message queues.
 *
 * Think of a segmentation, and allow the client to connect to only one of them.
 * The idea behind comes from STOMP, which allows setting a "host"-header
 * in the CONNECT-frame.
 *
 * This "host" header's value is the name of a given domain.
 *
 * I could have named this API "Host", though, this is very unintuitive and
 * would lead to misinterpretations and confusions.
 */
class STOMPEDE_API Domain
{
public:
	Domain();
	~Domain();

	bool contains(const std::string& destination) const;
	Channel* find(const std::string& destination) const;
	Channel* findOrCreate(const std::string& destination);

private:
	std::unordered_map<std::string, Channel*> channels_;
};

} // namespace stompede
