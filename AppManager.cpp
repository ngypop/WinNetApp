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

	//while(appManager.isOver() == false);
	
	return appManager.getErrorState();
}


AppManager::AppManager()  // @suppress("Class members should be properly initialized")
{
	over          = false;
	errorState    = -1;
	userIoHandler = new UserIoHandler();
	userIoHandler->launch(); // Returns when appManager is finished

	cout << "AppManager created\n";
}

void AppManager::shutdown(int _errorState) // @suppress("Ambiguous problem")
{
	over       = true;
	errorState = _errorState;
}

bool AppManager::isOver()
{
	return over;
}

int AppManager::getErrorState()
{
	return errorState;
}


AppManager::~AppManager()
{
	delete userIoHandler;
	cout << "AppManager destroyed\n";
}

