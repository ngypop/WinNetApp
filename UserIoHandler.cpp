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
	cout << "UserIoHandler created\n";
}

void UserIoHandler::userIoThread()
{
	int a = 0;

	while(a < 5)
	{
		cin >> a;
		cout << a << "\n";
	}
}

void UserIoHandler::launch()
{
	thread th (&UserIoHandler::userIoThread, this);
	th.join();
}



UserIoHandler::~UserIoHandler()
{
	cout << "UserIoHandler destroyed\n";
}

