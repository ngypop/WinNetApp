/*
 * Ping.cpp
 *
 *  Created on: 01.09.2018
 *      Author: Ingo
 */

#include <iostream>
#include <winsock2.h>
#include "Ping.h"

using namespace std;


Ping::Ping()
{
	name        = "ping";
	description = "Ping a target host with ICMP messages";
	pingStm.start();
}

void Ping::executeCommand(string _command)
{
	if(_command.find("help") == 0)
	{
		printHelpText();
	}
	else if(_command.find("init") == 0)
	{
		pingStm.process_event(e_init(this));
	}
	else if(_command.find("uninit") == 0)
	{
		pingStm.process_event(e_uninit());
	}
}

void Ping::printHelpText()
{
	cout << "Available commands: \n";
	cout << "'help'   - Display this help message \n";
	cout << "'init'   - Initialize Ping \n";
	cout << "'uninit' - Uninitialize Ping (Releases all network resources) \n";
}

int Ping::initialize()
{
	WSADATA wsa;

	cout << "Initializing winsock...";

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Failed. Error code: %d", WSAGetLastError();
		return 1;
	}
	cout << "initialized\n";


	// Create a socket
	// AF_INET:      Use IPv4. A separate socket would have to be created for any other family
	// SOCK_RAW:     A raw socket does not limit us to any connection type
	// IPPROTO_ICMP: Ping uses the ICMP protocol

	cout << "Creating socket...";
	if ((pingSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP) == INVALID_SOCKET))
	{
		cout << "Failed. Error code: %d \n", WSAGetLastError();
		if(WSAGetLastError() == 10013)
		{
			cout << "Application needs administrator rights to work!\n";
		}
		return 1;
	}
	cout << "Created\n";

	return 0;
}




