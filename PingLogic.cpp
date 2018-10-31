/*
 * PingSTM.cpp
 *
 *  Created on: 01.09.2018
 *      Author: Ingo
 */

#include "PingLogic.h"


#include <iostream>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>


using namespace std;



Ping::Ping()
{
	name        = "ping";
	description = "Ping a target host with ICMP messages";

	pingImpl   = new PingImplementation();
	pingParams = new PingParameters();

	pingStm = new t_pingStm(pingImpl, pingParams);

	pingStm->start();
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
		pingStm->process_event(e_init());
	}
	else if(_command.find("uninit") == 0)
	{
		pingStm->process_event(e_uninit());
	}
	else if(_command.find("setTarget") == 0)
	{
		if(substrings.size() > 1)
		{
			pingStm->process_event(e_setTarget(substrings[1]));
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
				pingStm->process_event(e_setTTL(value));
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
				pingStm->process_event(e_setTimeout(value));
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
	else if(_command.find("setCount") == 0)
	{
		if(substrings.size() > 1)
		{
			int value = atoi(substrings[1].c_str());

			if(value > -1)
			{
				pingStm->process_event(e_setCount(value));
			}
			else
			{
				cout << "Bad value for count: " << substrings[1] << endl;
			}
		}
		else
		{
			cout << "No value specified" << endl;
		}
	}
	else if(_command.find("setPayloadSize") == 0)
	{
		if(substrings.size() > 1)
		{
			int value = atoi(substrings[1].c_str());

			if(value > 0)
			{
				pingStm->process_event(e_setCount(value));
			}
			else
			{
				cout << "Bad value for size: " << substrings[1] << endl;
			}
		}
		else
		{
			cout << "No value specified" << endl;
		}
	}
	else if(_command.find("startPing") == 0)
	{
		pingStm->process_event(e_startPing());
	}
	else if(_command.find("stopPing") == 0)
	{
		pingStm->process_event(e_stopPing());
	}
	else
	{
		cout << "Command '" << _command << "' not available" << endl;
	}
}

void Ping::printHelpText()
{
	cout << "Available commands: " << endl;
	cout << "'help'                 - Display this help message " << endl;
	cout << "'init'                 - Initialize Ping " << endl;
	cout << "'uninit'               - Uninitialize Ping (Releases all network resources) " << endl;
	cout << "'setTarget target'     - Set target address. Current value: " << pingParams->targetName << endl;
	cout << "'setTTL TTL'           - Set TTL in milliseconds. Current value: " << pingParams->ttl << endl;
	cout << "'setTimeout timeout'   - Set response timeout in milliseconds. Current value: " << pingParams->recvTimeout << endl;
	cout << "'setCount count'       - Set number of ping requests to send. -1 for infinite. Current value: " << pingParams->count << endl;
	cout << "'setInterval interval' - Set interval by which pings are sent in milliseconds. Current value: " << pingParams->interval << endl;
	cout << "'setPayloadSize size'  - Set payload size of ping packets in bytes. Current value: " << pingParams->payloadSize << endl;
	cout << "'startPing'            - Start pinging" << endl;
	cout << "'stopPing'             - Stop pinging" << endl;
}

void Ping::setCount(int _count)
{
	cout << "Setting ping count to " << _count << "...";
	pingParams->count = _count;
	cout << "set" << endl;
}

void Ping::setInterval(unsigned int _interval)
{
	cout << "Setting ping interval to " << _interval << "ms...";
	pingParams->interval = _interval;
	cout << "set" << endl;
}

void Ping::setSize(unsigned int _size)
{
	cout << "Setting payload size to " << _size << "...";
	pingParams->payloadSize = _size;
	cout << "set" << endl;
}

void Ping::sendEvtByInterval()
{
	//while(pingActive)
	{
		pingStm->process_event(e_sendPing());
	}
}

