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
	else if(_command.find("setTarget") == 0)
	{
		if(substrings.size() > 1)
		{
			pingStm.process_event(e_setTarget(this, substrings[1]));
		}
		else
		{
			cout << "No target specified" << endl;
		}
	}
	else if(_command.find("setTTL") == 0)
	{
		if(substrings.size() > 1)
		{
			int value = atoi(substrings[1].c_str());

			if((value > 0) && (value < 256))
			{
				pingStm.process_event(e_setTTL(this, value));
			}
			else
			{
				cout << "Bad value for TTL: " << substrings[1] << endl;
			}
		}
		else
		{
			cout << "No value specified" << endl;
		}
	}
	else if(_command.find("setTimeout") == 0)
	{
		if(substrings.size() > 1)
		{
			int value = atoi(substrings[1].c_str());

			if(value > 0)
			{
				pingStm.process_event(e_setTimeout(this, value));
			}
			else
			{
				cout << "Bad value for timeout: " << substrings[1] << endl;
			}
		}
		else
		{
			cout << "No value specified" << endl;
		}
	}
	else
	{
		cout << "Command '" << _command << "' not availble" << endl;
	}
}

void Ping::printHelpText()
{
	cout << "Available commands: " << endl;
	cout << "'help'               - Display this help message " << endl;
	cout << "'init'               - Initialize Ping " << endl;
	cout << "'uninit'             - Uninitialize Ping (Releases all network resources) " << endl;
	cout << "'setTarget target'   - Set target address. Current value: " << targetName << endl;
	cout << "'setTTL TTL'         - Set TTL in milliseconds. Current value: " << ttl << endl;
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
	setTTL(ttl);
	setRecvTimeout(recvTimeout);
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

void Ping::setTarget(string _target)
{

	ADDRINFOA *hostAddr = NULL;
	bool success = false;

	// Turn first passed parameter into an IP address to ping
	unsigned int addr = inet_addr(_target.c_str());

	if (addr != INADDR_NONE) {
		// It was a dotted quad number, so save result
		dest.sin_addr.s_addr = addr;
		dest.sin_family = AF_INET;
		success = true;
	}
	else
	{
		// It was not in dotted quad form, so try and look it up
		if(getaddrinfo(_target.c_str(), 0, NULL, &hostAddr) == SOCKET_ERROR)
		{
			cout << "getaddrinfo() failed. Error code: " << WSAGetLastError() << endl;
		}
		else if (hostAddr != 0)
		{
			// Found an address for that host, so save it
			memcpy(&dest, hostAddr->ai_addr, sizeof(SOCKADDR));
			cout << "Found address: " << inet_ntoa(dest.sin_addr) << endl;
			success = true;
		}
		else
		{
			// Not a recognized hostname either!
			cout << "Failed to interpret host address: " << _target << endl;
		}
	}
	if(success)
	{
		targetName = _target;
	}
}

void Ping::setTTL(unsigned int _ttl)
{
	// Set the TTL value associated with IP traffic on the socket.
	cout << "Setting socket option TTL to " << _ttl << "...";
	if (setsockopt(pingSocket, IPPROTO_IP, IP_TTL, (const char*)&_ttl,	sizeof(_ttl)) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return;
	}
	cout << "set" << endl;
}


void Ping::setRecvTimeout(unsigned int _recvTimeout)
{
	cout << "Setting socket option receive timeout to " << _recvTimeout << "ms...";
	if (setsockopt(pingSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&_recvTimeout, sizeof(_recvTimeout)) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return;
	}
	cout << "set" << endl;
}

