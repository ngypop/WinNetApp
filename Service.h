/*
 * Service.h
 *
 *  Created on: 01.09.2018
 *      Author: Ingo
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <string>

using namespace std;


class Service
{
public:
	string getName();
	string getDescription();
	virtual void executeCommand(string _command) = 0;

protected:
	string name;
	string description;
};

#endif /* SERVICE_H_ */
