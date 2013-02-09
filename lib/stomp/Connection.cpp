#include <stompede/stomp/Connection.h>
#include <stompede/stomp/Broker.h>

namespace stompede {
namespace stomp {

Connection::Connection(Broker* broker, int fd) :
	broker_(broker),
	io_(broker_->loop()),
	subscriptions_()
{
	io_.set<Connection, &Connection::io>(this);

	reset(fd);
}

Connection::~Connection()
{
	::close(io_.fd);
}

void Connection::reset(int fd)
{
	Reader::reset();

	io_.set(fd, ev::READ);
	io_.start();
}

// {{{ Network I/O
void Connection::io(ev::io& io, int revents)
{
	if ((revents & ev::READ) && !readSome())
		return;

	if ((revents & ev::WRITE) && !writeSome())
		return;
}

bool Connection::readSome()
{
	return false;
}

bool Connection::writeSome()
{
	return false;
}

void Connection::timeout(ev::timer& timer, int revents)
{
}
// }}}
// {{{ client frame handlers
void Connection::onConnect()
{
}

void Connection::onDisconnect()
{
}

void Connection::onSend()
{
}

void Connection::onSubscribe()
{
}

void Connection::onUnsubscribe()
{
}

void Connection::onBegin()
{
}

void Connection::onCommit()
{
}

void Connection::onAck()
{
}

void Connection::onNack()
{
}
// }}}
// {{{ server frame actions
void Connection::sendConnected()
{
}

void Connection::sendMessage()
{
}

void Connection::sendReceipt()
{
}

void Connection::sendError()
{
}
// }}}
// {{{ direct output frame manipulation
void Connection::writeBegin(const std::string& name)
{
}

void Connection::writeHeader(const std::string& name, const std::string& value)
{
}

void Connection::writeBody(const std::string& value)
{
}

void Connection::writeEnd()
{
}
// }}}

} // namespace stomp
} // namespace stompede
