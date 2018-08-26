/*
 * DijkstraTest.h
 *
 *  Created on: 26.08.2018
 *      Author: Ingo
 */

#ifndef DIJKSTRATEST_H_
#define DIJKSTRATEST_H_

#include <map>

using namespace std;

class DijkstraTest
{
public:
	DijkstraTest();
	virtual ~DijkstraTest();
	void testDijkstra();

private:
	void runDijkstra(int startNode, map<int, map<int, int>> &graph, map<int, int> &result);
};

#endif /* DIJKSTRATEST_H_ */
