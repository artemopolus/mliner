#include "zmq/exmliner.h"
#include <iostream>

#define EX_MLINER_ZMQ_HEADER_STR "Quote"

exMlinerZMQ::exMlinerZMQ()
{
	RxBuffer = nullptr;
	TxBuffer = nullptr;
	RxBuffLen = 0;
	TxBuffLen = 0;
	isInited = false;
	isTxInited = false;
	isRxInited = false;

	std::cout << "Init exMliner" << std::endl;
}

int exMlinerZMQ::init()
{
	if (!isInited)
	{
		isInited = true;
		std::cout << "exMlinerZMQ is inited" << std::endl;

		TxContext = zmq::context_t(1);
		TxSocket = zmq::socket_t(TxContext, zmq::socket_type::pub);



		RxContext = zmq::context_t(1);
		RxSocket = zmq::socket_t(RxContext, zmq::socket_type::sub);
		int reconnectTime = 10;
		RxSocket.setsockopt(ZMQ_RECONNECT_IVL, &reconnectTime, sizeof(reconnectTime));
		int ivl = 5 * 1000;
		int ttl = 2 * 60 * 1000;
		int timeout = 5 * 1000;
		RxSocket.setsockopt(ZMQ_HEARTBEAT_IVL, &ivl, sizeof(ivl));
		RxSocket.setsockopt(ZMQ_HEARTBEAT_TTL, &ttl, sizeof(ttl));
		RxSocket.setsockopt(ZMQ_HEARTBEAT_TIMEOUT, &timeout, sizeof(timeout));

		RxSocket.setsockopt(ZMQ_SUBSCRIBE, EX_MLINER_ZMQ_HEADER_STR, strlen(EX_MLINER_ZMQ_HEADER_STR));
	}
	return 0;
}

int exMlinerZMQ::send(uint8_t * data, uint16_t datalen)
{
	if(isRxInited)
	{
		//std::cout << "exMliner transmit: ";

		zmq::message_t header(strlen(EX_MLINER_ZMQ_HEADER_STR));
		memcpy(header.data(),EX_MLINER_ZMQ_HEADER_STR, strlen(EX_MLINER_ZMQ_HEADER_STR));
		TxSocket.send(header, zmq::send_flags::sndmore);

		zmq::message_t datapack(datalen);
		memcpy(datapack.data(), &data[0], datalen);
		/*for (int i = 0; i < datalen; i++)
			std::cout << " " << (int)data[i];*/
		TxSocket.send(datapack, zmq::send_flags::dontwait);
		std::cout << std::endl;
		return 1;
		
	}
	return 0;

}
void exMlinerZMQ::setName(int type, const char* name)
{
	if (type == 0)
	{
		TxPort = name;
	}
	else
	{
		RxPort = name;
	}
}
void exMlinerZMQ::connectToPort(const char* portname)
{
	if (!isRxInited)
		isRxInited = true;

	RxSocket.connect(portname);

	RecvPortNames.push_back(std::string(portname));

}
void exMlinerZMQ::createServer(const char* portname)
{
	if (!isTxInited)
	{
		isTxInited = true;
		TxSocket.bind(portname);
		TxPort = portname;
	}
}
int exMlinerZMQ::recv(uint8_t * data, uint16_t datalen)
{
	if(isRxInited)
	{
		zmq::message_t reply;
		//check
		RxSocket.recv(reply, zmq::recv_flags::dontwait);
		//std::cout << "exMliner receive: " << reply.size();
		//data
		RxSocket.recv(reply, zmq::recv_flags::dontwait);
		//std::cout << "data:" << reply.size();
		int len = datalen > reply.size() ? reply.size() : datalen;
		memcpy(&data[0], reply.data(), len);
		//std::cout << std::endl;
		//for (int i = 0; i < datalen; i++)
		//	std::cout << " " << (int)data[i];
		//std::cout << std::endl;
		return len;
	}
	return 0;

}