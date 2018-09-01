/*
 * UserIoHandler.cpp
 *
 *  Created on: 23.08.2018
 *      Author: Ingo
 */
#include <iostream>
#include <thread>
#include "UserIoHandler.h"

using namespace std;

UserIoHandler::UserIoHandler()
{
	shutdown = false;
	activeService = 0;
}

void UserIoHandler::registerService(Service *_service)
{
	services.insert(make_pair(_service->getName(), _service));
}

void UserIoHandler::userIoThread()
{
	cout << "Type 'help' for help!\n";
	while(!shutdown)
	{
		handleCommandPrompt();

	}
}

int UserIoHandler::launch()
{
	thread th (&UserIoHandler::userIoThread, this);
	th.join();

	return 0; // Later a proper return value can be implemented
}

void UserIoHandler::handleCommandPrompt()
{
	string input;
	map<string, Service*>::iterator iter;

	if(activeService == 0)
	{
		cout << "WinNetApp> ";
	}
	else
	{
		cout << "WinNetApp\\" << activeService->getName() << "> ";
	}
	getline(cin, input);

	if(input.find("shutdown") == 0)
	{
		shutdown = true;
	}
	else
	{
		iter = services.find(input);
		if(iter == services.end())
		{
			cout << "Service '" << input << "' not found \n";
		}
		else
		{
			activeService = (*iter).second;
		}
	}
}


