/*
 * PingImplementation.cpp
 *
 *  Created on: 23.10.2018
 *      Author: Ingo
 */

#include "PingImplementation.h"


PingImplementation::PingImplementation()
{
	// TODO Auto-generated constructor stub

}

PingImplementation::~PingImplementation()
{
	// TODO Auto-generated destructor stub
}

void PingImplementation::initialize(unsigned int _ttl, unsigned int _recvTimeout, std::function<void()> callbackSuccess, std::function<void()> callbackFail)
{
	WSADATA wsa;

	cout << "Initializing winsock...";

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		callbackFail();
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
		callbackFail();
		return;
	}
	cout << "Created" << endl;

	// Configure socket
	if(setTTL(_ttl) && setRecvTimeout(_recvTimeout))
	{
		callbackSuccess();
		return;
	}
}

bool PingImplementation::uninitialize()
{
	// Close socket
	cout << "Closing socket...";
	if(closesocket(pingSocket) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return false;
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
		return false;
	}
	else
	{
		cout << "Done" << endl;
	}
	return true;
}

bool PingImplementation::setTarget(string _target)
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
		return true;
	}
	return false;
}

bool PingImplementation::setTTL(unsigned int _ttl)
{
	// Set the TTL value associated with IP traffic on the socket.
	cout << "Setting socket option TTL to " << _ttl << "...";
	if (setsockopt(pingSocket, IPPROTO_IP, IP_TTL, (const char*)&_ttl,	sizeof(_ttl)) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return false;
	}
	cout << "set" << endl;
	return true;
}

bool PingImplementation::setRecvTimeout(unsigned int _recvTimeout)
{
	cout << "Setting socket option receive timeout to " << _recvTimeout << "ms...";
	if (setsockopt(pingSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&_recvTimeout, sizeof(_recvTimeout)) == SOCKET_ERROR)
	{
		cout << "Failed. Error code: " << WSAGetLastError() << endl;
		return false;
	}
	cout << "set" << endl;
	return true;
}


void PingImplementation::sendPing()
{

}

void PingImplementation::processResponse()
{
}
