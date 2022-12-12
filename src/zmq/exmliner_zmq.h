#pragma once

#include "zmq/exmliner.h"



class exMlinerZMQclient : public exMlinerZMQ
{
public:
	virtual ~exMlinerZMQclient() override;
	virtual int init() override;


};

class exMlinerZMQserver : public exMlinerZMQ
{
public:
	virtual ~exMlinerZMQserver() override;
	virtual int init() override;

};
