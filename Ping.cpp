/*
 * Ping.cpp
 *
 *  Created on: 01.09.2018
 *      Author: Ingo
 */

#include <iostream>
#include <vector>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include "Ping.h"

using namespace std;


Ping::Ping()
{
	name        = "ping";
	description = "Ping a target host with ICMP messages";
	ttl         = 128;
	recvTimeout = 1000;
	pingStm.start();
}

void Ping::executeCommand(string _command)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

	// Separate command into space-divided substrings
	std::vector<std::string> substrings;
	boost::char_separator<char> sep(" ");
	tokenizer tokens(_command, sep);

	BOOST_FOREACH(std::string const& token, tokens)
	{
		substrings.push_back(token);
	}


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
		pingStm.process_event(e_uninit(this));
	}
	else if(_command.find("setTTL") == 0)
	{
		int value = atoi(substrings[1].c_str());

		if((value > 0) && (value < 256))
		{
			pingStm.process_event(e_setTTL(value));
		}
		else
		{
			cout << "Bad value for TTL: " << substrings[1] << endl;
		}
	}
}

void Ping::printHelpText()
{
	cout << "Available commands: " << endl;
	cout << "'help'               - Display this help message " << endl;
	cout << "'init'               - Initialize Ping " << endl;
	cout << "'uninit'             - Uninitialize Ping (Releases all network resources) " << endl;
	cout << "'setTTL TTL'         - Set TTL in milliseconds. Current value: " << ttl << endl;
	cout << "'setTarget target'   - Set target address. Current value: " << targetName << endl;
	cout << "'setTimeout timeout' - Set response timeout in milliseconds. Current value: " << recvTimeout << endl;
}

void Ping::initialize()
{
	WSADATA wsa;

	cout << "Initializing winsock...";

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return;
	}
	cout << "initialized" << endl;


	// Create a socket
	// AF_INET:      Use IPv4. A separate socket would have to be created for any other family
	// SOCK_RAW:     A raw socket does not limit us to any connection type
	// IPPROTO_ICMP: Ping uses the ICMP protocol

	cout << "Creating socket...";
	if ((pingSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == INVALID_SOCKET)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		if(WSAGetLastError() == 10013)
		{
			cout << "Application needs administrator rights to work!" << endl;
		}
		return;
	}
	cout << "Created" << endl;

	// Configure socket
	// Set the TTL value associated with IP multicast traffic on the socket.
	cout << "Setting socket option TTL to " << ttl << "...";
	if (setsockopt(pingSocket, IPPROTO_IP, IP_TTL, (const char*)&ttl,	sizeof(ttl)) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return;
	}
	cout << "set" << endl;

	cout << "Setting socket option receive timeout to " << recvTimeout << "ms...";
	if (setsockopt(pingSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&recvTimeout, sizeof(recvTimeout)) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return;
	}
	cout << "set" << endl;
}

void Ping::uninitialize()
{
	// Close socket
	cout << "Closing socket...";
	if(closesocket(pingSocket) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
	}
	else
	{
		cout << "Closed" << endl;
	}

	// Cleanup
	cout << "Cleaning up...";
	if(WSACleanup() != 0)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
	}
	else
	{
		cout << "Done" << endl;
	}
}




