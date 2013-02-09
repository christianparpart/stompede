#include <stompede/stomp/Broker.h>
#include <stompede/stomp/Connection.h>
#include <stompede/sysconfig.h>
#include <ev++.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

namespace stompede {
namespace stomp {

Broker::Broker(ev::loop_ref loop) :
	loop_(loop),
	errorText_(),
	domains_(),
	defaultDomain_(nullptr),
	address_(),
	port_(-1),
	addressFamily_(0),
	io_(loop_),
	deferAccept_(false),
	supportsAccept4_(false)
{
	io_.set<Broker, &Broker::accept>(this);
}

Broker::~Broker()
{
	kill();
}

bool Broker::setupWorkers(size_t count)
{
	return false;
}

bool Broker::setupListener(const char* address, int port, int backlog)
{
	addrinfo* res = nullptr;
	addrinfo hints;
	addrinfo* ri = nullptr;
	int rc;
	int fd = -1;
	in6_addr saddr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rc = inet_pton(AF_INET, address, &saddr)) == 1) {
		hints.ai_family = AF_INET;
		hints.ai_flags |= AI_NUMERICHOST;
	} else if ((rc = inet_pton(AF_INET6, address, &saddr)) == 1) {
		hints.ai_family = AF_INET6;
		hints.ai_flags |= AI_NUMERICHOST;
	} else {
		switch (rc) {
			case -1: // errno is set properly
				errorText_ = strerror(errno);
				break;
			case 0:  // invalid network addr format
			default: // unknown error
				errorText_ = strerror(EINVAL);
				break;
		}
		return false;
	}

	char sport[16];
	snprintf(sport, sizeof(sport), "%d", port);

	if ((rc = getaddrinfo(address, sport, &hints, &res)) != 0) {
		errorText_ = gai_strerror(rc);
		goto err;
	}

	for (ri = res; ri != nullptr; ri = ri->ai_next) {
		fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (fd < 0)
			goto syserr;

		rc = 1;

#if defined(SO_REUSEADDR)
		if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &rc, sizeof(rc)) < 0)
			goto syserr;
#endif

#if defined(TCP_QUICKACK)
		if (::setsockopt(fd, SOL_TCP, TCP_QUICKACK, &rc, sizeof(rc)) < 0)
			goto syserr;
#endif

#if defined(TCP_DEFER_ACCEPT)
		deferAccept_ = ::setsockopt(fd, SOL_TCP, TCP_DEFER_ACCEPT, &rc, sizeof(rc)) == 0;
#endif

		if (::bind(fd, res->ai_addr, res->ai_addrlen) < 0)
			goto syserr;

		if (::listen(fd, backlog) < 0)
			goto syserr;

		// XXX success
		addressFamily_ = res->ai_family;
		address_ = address;
		port_ = port;
		io_.set(fd, ev::READ);

		return true;
	}

syserr:
	errorText_ = strerror(errno);

err:
	if (res)
		freeaddrinfo(res);

	if (fd >= 0)
		::close(fd);

	return false;
}

void Broker::accept(ev::io& io, int)
{
	bool flagged = false;
	int cfd;

#if defined(HAVE_ACCEPT4) && defined(WITH_ACCEPT4)
	if (supportsAccept4_) {
		cfd = ::accept4(io.fd, nullptr, 0, SOCK_CLOEXEC | SOCK_NONBLOCK);

		if (cfd < 0 && errno == ENOSYS) {
			cfd = ::accept(io.fd, nullptr, 0);
			supportsAccept4_ = false;
		} else {
			flagged = true;
		}
	} else {
		cfd = ::accept(io.fd, nullptr, 0);
	}
#else
	cfd = ::accept(io.fd, nullptr, 0);
#endif

	if (cfd < 0) {
		switch (errno) {
			case EINTR:
			case EAGAIN:
#if (EAGAIN != EWOULDBLOCK)
			case EWOULDBLOCK:
#endif
				return;
			default:
				fprintf(stderr, "error in accept(): %s\n", strerror(errno));
				return;
		}
	}

	if (!flagged && fcntl(cfd, F_SETFL, fcntl(cfd, F_GETFL) | O_NONBLOCK | O_CLOEXEC) < 0)
		goto err;

	fprintf(stdout, "client connected: fd = %d\n", cfd);

	new Connection(this, cfd);

	return;

err:
	::close(cfd);
}

bool Broker::start()
{
//	if (io_.fd < 0)
//		return false;

	io_.start();
	return true;
}

void Broker::stop()
{
	io_.stop();
}

void Broker::kill()
{
	stop();

	// TODO 1.) kill all active connections
	// TODO 2.) kill all workers
}


} // namespace stomp
} // namespace stompede
