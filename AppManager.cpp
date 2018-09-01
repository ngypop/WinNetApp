/*
 * UserIoHandler.cpp
 *
 *  Created on: 23.08.2018
 *      Author: Ingo
 */
 
#include <iostream>
#include "AppManager.h"
#include "UserIoHandler.h"

using namespace std;

int main()
{
	AppManager appManager;

	appManager.go();

	return appManager.getErrorState();
}


AppManager::AppManager()
{
	version ="0.1";
	errorState    = -1;

	// Create user Io Handler
	userIoHandler = new UserIoHandler();

	// Create and register services
    ping = new Ping("ping");
    userIoHandler->registerService(ping);
}

void AppManager::go()
{
	cout << "WinNetApp version " << version << "\n";
	errorState = userIoHandler->launch(); // Returns when appManager is finished
	cout << "Bye.\n";
}


int AppManager::getErrorState()
{
	return errorState;
}


AppManager::~AppManager()
{
	delete userIoHandler;
}

