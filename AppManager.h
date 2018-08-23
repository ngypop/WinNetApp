/*
 * AppManager.h
 *
 *  Created on: 22.08.2018
 *      Author: Ingo
 */

#ifndef APPMANAGER_H_
#define APPMANAGER_H_

#include <string>
#include "UserIoHandler.h"

using namespace std;

class AppManager {
public:
	AppManager();
	virtual ~AppManager();

	int getErrorState();
	void go();

private:
	int errorState;
	UserIoHandler *userIoHandler;
	string version;
};

#endif /* APPMANAGER_H_ */
