#pragma once

#include <stompede/Api.h>
#include <unordered_map>
#include <string>
#include <ev++.h>

namespace stompede {
	class Channel;
}

namespace stompede {
namespace stomp {

class Broker;

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
	Channel* find(const std::string& destination);

private:
	std::unordered_map<std::string, Channel*> channels_;
};

/*! A worker thread serving broker connections.
 */
class STOMPEDE_API Worker
{
private:
	pthread_t id_;
	Broker* broker_;
	ev::loop_ref loop_;
};

/*! A Broker instance.
 */
class STOMPEDE_API Broker
{
public:
	Broker(ev::loop_ref loop);
	~Broker();

	bool setupWorkers(size_t count);
	bool setupListener(const char* bind = "0.0.0.0", int port = 61613, int backlog = 128);

	bool start();
	void stop();
	void kill();

	const std::string& errorText() const { return errorText_; }

	ev::loop_ref loop() const { return loop_; }

private:
	void accept(ev::io& io, int revents);

private:
	ev::loop_ref loop_;

	std::string errorText_;

	//! map of registered domains
	std::unordered_map<std::string, Domain*> domains_;
	Domain* defaultDomain_;

	std::string address_;  //!< tcp listener address
	int port_;             //!< tcp listener port
	int addressFamily_;    //!< address family
	ev::io io_;            //!< I/O watcher to listener socket
	bool deferAccept_;     //!< true, if TCP_DEFER_ACCEPT enabled
	bool supportsAccept4_; //!< true, if is accept4() supported
};

} // namespace stomp
} // namespace stompede
