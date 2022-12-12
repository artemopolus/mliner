#include "zmq/exmliner_zmq.h"

#include <iostream>


exMlinerZMQclient::~exMlinerZMQclient()
{
}
int exMlinerZMQclient::init()
{
	if (!isInited)
	{
		TxContext = zmq::context_t(1);
		//Socket = zmq::socket_t(Context, zmq::socket_type::req);

		std::cout << "Connecting to hello world server..." << std::endl;
		//Socket.connect("tcp://localhost:5555");
		isInited = true;
	}
	return 0;
}

exMlinerZMQserver::~exMlinerZMQserver()
{
	//Socket.close();
}
int exMlinerZMQserver::init()
{
	if (!isInited)
	{
		TxContext = zmq::context_t(2);
		//Socket = zmq::socket_t(Context, zmq::socket_type::pub);
		//Socket.bind("tcp://*:5555");
		isInited = true;
	}
	return 0;
}
