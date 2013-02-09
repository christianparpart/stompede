#pragma once

#include <stompede/Api.h>
#include <unordered_map>
#include <string>
#include <ev++.h>

namespace stompede {
	class Domain;
}

namespace stompede {
namespace stomp {

class Broker;

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

	Domain* createDomain(const std::string& name);

	void setDefaultDomain(Domain* domain);
	Domain* defaultDomain() const { return defaultDomain_; }

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

	//! pointer to default domain or NULL if not provided.
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
