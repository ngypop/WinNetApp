/*
 * PingParameters.h
 *
 *  Created on: 31.10.2018
 *      Author: Ingo
 */

#ifndef PINGPARAMETERS_H_
#define PINGPARAMETERS_H_

#include <string>

using namespace std;

class PingParameters
{
public:

	PingParameters();

	unsigned int ttl;
	int          recvTimeout;
	int          count;
	unsigned int interval;
	unsigned int payloadSize;

	string targetName;
};



#endif /* PINGPARAMETERS_H_ */
