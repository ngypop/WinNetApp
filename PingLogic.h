/*
 * Ping.h
 *
 *  Created on: 01.09.2018
 *      Author: Ingo
 */

#ifndef PINGLOGIC_H_
#define PINGLOGIC_H_


#include "Service.h"
#include "PingImplementation.h"
#include "PingParameters.h"
#include "PingStateMachine.hpp"

class Ping : public Service
{
public:
	Ping();
	void executeCommand(string _command);

private:
	void printHelpText();
	void setCount(int _count);
	void setInterval(unsigned int _interval);
	void setSize(unsigned int _size);
	void sendEvtByInterval();

	t_pingStm* pingStm;

	PingImplementation* pingImpl;
	PingParameters*     pingParams;
};

#endif /* PINGLOGIC_H_ */
