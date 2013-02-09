#pragma once

#include <stompede/Api.h>
#include <stompede/Buffer.h>
#include <string>
#include <utility>
#include <list>

namespace stompede {

class Domain;

//! Message reliability.
enum class Reliability {
	//! Message may be delivered once, never more, and maybe never.
	AtMostOnce,

	//! Message is exactly once delivered, never more, never less.
	ExactlyOnce,

	//! Message gets delivered at least once, never less, and maybe more.
	AtLeastOnce,
};

//! Message durability.
enum class Durability {
	//! message is held in memory only, thus may get lost.
	Volatile,

	//!< message is stored on disk for consistency and recovery until someone took the message.
	Durable
};

/*! A message object.
 *
 * A message is a deliverable generic entity that can be passed from one endpoint to another.
 *
 * \see Channel, Transaction, Domain, Broker
 */
class STOMPEDE_API Message
{
public:
	explicit Message(Domain* domain);
	Message(const Message&) = default;
	~Message();

	Domain* domain() const { return domain_; }

	std::list<std::pair<std::string, std::string>>& headers() { return headers_; }
	const std::list<std::pair<std::string, std::string>>& headers() const { return headers_; }
	void addHeader(const std::string& name, const std::string& value);

	Buffer& body() { return body_; }
	const Buffer& body() const { return body_; }
	void setBody(Buffer&& body) { body_ = std::move(body); }
	void setBody(const Buffer& body) { body_ = body; }

	Reliability reliability() const { return reliability_; }
	void setReliability(Reliability reliability) { reliability_ = reliability; }

private:
	Domain* domain_;
	std::list<std::pair<std::string, std::string>> headers_;
	Buffer body_;

	Reliability reliability_;
};

} // namespace stompede
