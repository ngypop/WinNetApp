/*
 * PingParameters.cpp
 *
 *  Created on: 31.10.2018
 *      Author: Ingo
 */

#include "PingParameters.h"

PingParameters::PingParameters()
{
	ttl         = 128;
	recvTimeout = 1000;
	count       = -1;
	interval    = 1000;
	payloadSize = 32; // Same as Windows internal ping command default
}


