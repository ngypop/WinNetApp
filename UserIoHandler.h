/*
 * UserIoHandler.h
 *
 *  Created on: 23.08.2018
 *      Author: Ingo
 */

#ifndef USERIOHANDLER_H_
#define USERIOHANDLER_H_

using namespace std;

class UserIoHandler
{
public:
	UserIoHandler();
	virtual ~UserIoHandler();
	void launch();

private:
	void userIoThread();
};

#endif /* USERIOHANDLER_H_ */
