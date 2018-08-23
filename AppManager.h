/*
 * AppManager.h
 *
 *  Created on: 22.08.2018
 *      Author: Ingo
 */

#ifndef APPMANAGER_H_
#define APPMANAGER_H_

#include "UserIoHandler.h"

class AppManager {
public:
	AppManager();
	virtual ~AppManager();

	void shutdown(int _errorState);
	bool isOver();
	int getErrorState();
	void launch();

private:
	bool over;
	int errorState;
	UserIoHandler* userIoHandler;
};

#endif /* APPMANAGER_H_ */
