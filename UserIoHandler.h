/*
 * UserIoHandler.h
 *
 *  Created on: 23.08.2018
 *      Author: Ingo
 */

#ifndef USERIOHANDLER_H_
#define USERIOHANDLER_H_

#include <map>
#include "Service.h"

using namespace std;

class UserIoHandler
{
public:
	UserIoHandler();
	void registerService(Service *_service);
	int launch();

private:
	void userIoThread();
	void printWelcomeMessage();
	void handleCommandPrompt();
	bool shutdown;
	map<string, Service*> services;
	Service* activeService;
};

#endif /* USERIOHANDLER_H_ */
