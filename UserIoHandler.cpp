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
}

void UserIoHandler::userIoThread()
{
	cout << "Type 'help' for help!\n";
	while(!shutdown)
	{
		getNextCommand();
	}
}

int UserIoHandler::launch()
{
	thread th (&UserIoHandler::userIoThread, this);
	th.join();

	return 0; // Later a proper return value can be implemented
}

void UserIoHandler::getNextCommand()
{
	string input;

	cout << "WinNetApp> ";
	getline(cin, input);

	if(input.find("shutdown") == 0)
	{
		shutdown = true;
	}
}


UserIoHandler::~UserIoHandler()
{
}

