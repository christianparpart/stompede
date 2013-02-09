#include <cstdio>
#include <ev++.h>
#include <stompede/stomp/Reader.h>
#include <stompede/stomp/Broker.h>

int main(int argc, const char* argv[])
{
	ev::default_loop loop(0);
	stompede::stomp::Broker broker(loop);

	broker.setupWorkers(4);
	broker.setupListener("0.0.0.0", 61613);
	broker.start();

	loop.run();

	return 0;
}
