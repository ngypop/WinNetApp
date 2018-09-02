/*
 * Ping.h
 *
 *  Created on: 01.09.2018
 *      Author: Ingo
 */

#ifndef PING_H_
#define PING_H_

#include <winsock2.h>
#include "Service.h"

class Ping : public Service
{
public:
	Ping();
	void executeCommand(string _command);

private:
	void printHelpText();
	int initialize();

	SOCKET pingSocket;
};

#endif /* PING_H_ */
