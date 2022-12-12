#pragma once

#include <zmq.hpp>
#include <string>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)	Sleep(n)
#endif

class exMlinerZMQ
{
public: //function
	exMlinerZMQ();
	virtual ~exMlinerZMQ(){};
	virtual int init();

	int recv(uint8_t * data, uint16_t datalen);
	int send(uint8_t * data, uint16_t datalen);

	void setName(int type, const char* name);

	void connectToPort(const char* portname);
	void createServer(const char* portname);

protected: //variables
	bool isInited;
	bool isRxInited;
	bool isTxInited;
	std::vector<std::string> RecvPortNames;
	zmq::context_t RxContext;
	zmq::context_t TxContext;
	zmq::socket_t TxSocket;
	zmq::socket_t RxSocket;
public: //variables

	std::string RxPort;
	std::string TxPort;

	uint8_t* RxBuffer;
	uint32_t * RxBuffLen;
	uint8_t* TxBuffer;
	uint32_t * TxBuffLen;
};


