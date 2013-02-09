#pragma once

#include <stompede/Api.h>
#include <stompede/stomp/Reader.h>
#include <unordered_map>
#include <string>
#include <ev++.h>

namespace stompede {
	class Channel;
}

namespace stompede {
namespace stomp {

class Broker;

/*! Represents a streaming connection from the remote endpoint to our broker instance.
 */
class STOMPEDE_API Connection :
	public Reader
{
public:
	Connection(Broker* broker, int fd);
	~Connection();

	void reset(int fd);

	// client frame handlers
	void onConnect();
	void onDisconnect();
	void onSend();
	void onSubscribe();
	void onUnsubscribe();
	void onBegin();
	void onCommit();
	void onAck();
	void onNack();

	// server frame actions
	void sendConnected();
	void sendMessage();
	void sendReceipt();
	void sendError();

	// direct output frame manipulation
	void writeBegin(const std::string& name);
	void writeHeader(const std::string& name, const std::string& value);
	void writeBody(const std::string& value);
	void writeEnd();

private:
	// networking
	void io(ev::io& io, int revents);
	bool readSome();
	bool writeSome();
	void timeout(ev::timer& timer, int revents);

private:
	Broker* broker_;
	ev::io io_;

	std::unordered_map<unsigned, Channel*> subscriptions_;
};

} // namespace stomp
} // namespace stompede
