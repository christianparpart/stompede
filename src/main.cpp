#include <cstdio>
#include <ev++.h>
#include <stompede/stomp/Broker.h>

int main(int argc, const char* argv[])
{
	// TODO: args & signal handling

	ev::default_loop loop(0);
	stompede::stomp::Broker broker(loop);

	auto domain = broker.createDomain("default");
	broker.setDefaultDomain(domain);

	broker.setupWorkers(4);
	broker.setupListener("0.0.0.0", 61613);
	broker.start();

	loop.run();

	return 0;
}
