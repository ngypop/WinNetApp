/*
 * PingImplementation.h
 *
 *  Created on: 23.10.2018
 *      Author: Ingo
 */

#ifndef PINGIMPLEMENTATION_H_
#define PINGIMPLEMENTATION_H_

using namespace std;

#include <iostream>
#include <functional>
#include <winsock2.h>
#include <Ws2tcpip.h>


class PingImplementation
{
public:
	PingImplementation();
	virtual ~PingImplementation();

	void initialize(unsigned int _ttl, unsigned int _recvTimeout, std::function<void()> callbackSuccess, std::function<void()> callbackFail);
	bool uninitialize();
	bool setTarget(string _target);
	bool setRecvTimeout(unsigned int _recvTimeout);
	bool setTTL(unsigned int _ttl);
	void sendPing();
	void processResponse();


	SOCKET pingSocket;

	struct sockaddr_in source, dest;

};

#endif /* PINGIMPLEMENTATION_H_ */
