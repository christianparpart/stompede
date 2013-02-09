#pragma once

#include <stompede/Api.h>
#include <stompede/Buffer.h>
#include <string>
#include <utility>
#include <list>

namespace stompede {

class STOMPEDE_API Message
{
public:
	Message() = default;
	Message(const Message&) = default;

	std::list<std::pair<std::string, std::string>>& headers() { return headers_; }
	const std::list<std::pair<std::string, std::string>>& headers() const { return headers_; }
	Buffer& body() { return body_; }
	const Buffer& body() const { return body_; }

private:
	std::list<std::pair<std::string, std::string>> headers_;
	Buffer body_;
};

} // namespace stompede
