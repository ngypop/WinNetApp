/*
 * DijkstraTest.cpp
 *
 *  Created on: 26.08.2018
 *      Author: Ingo
 */
#include <set>
#include <climits>
#include <iostream>
#include "DijkstraTest.h"

DijkstraTest::DijkstraTest()
{
	// TODO Auto-generated constructor stub

}

void DijkstraTest::testDijkstra()
{
	map<int, int> result;

	map<int, map<int, int>> graph;

	// Edges of vertex 1
	map<int, int> vert1Edges;
	vert1Edges.insert(pair<int, int>(1, 0));
	vert1Edges.insert(pair<int, int>(2, 4));
	vert1Edges.insert(pair<int, int>(3, 2));
	graph.insert(make_pair(1, vert1Edges));

	// Edges of vertex 2
	map<int, int> vert2Edges;
	vert2Edges.insert(pair<int, int>(2, 0));
	vert2Edges.insert(pair<int, int>(2, 0));
	vert2Edges.insert(pair<int, int>(3, 3));
	vert2Edges.insert(pair<int, int>(4, 2));
	vert2Edges.insert(pair<int, int>(5, 3));
	graph.insert(make_pair(2, vert2Edges));

	// Edges of vertex 3
	map<int, int> vert3Edges;
	vert3Edges.insert(pair<int, int>(3, 0));
	vert3Edges.insert(pair<int, int>(2, 1));
	vert3Edges.insert(pair<int, int>(4, 4));
	vert3Edges.insert(pair<int, int>(5, 5));
	graph.insert(make_pair(3, vert3Edges));

	// Edges of vertex 4
	map<int, int> vert4Edges;
	vert4Edges.insert(pair<int, int>(4, 0));
	graph.insert(make_pair(4, vert4Edges));

	// Edges of vertex 5
	map<int, int> vert5Edges;
	vert5Edges.insert(pair<int, int>(5, 0));
	vert5Edges.insert(pair<int, int>(4, 1));
	graph.insert(make_pair(5, vert5Edges));

	runDijkstra(1, graph, result);

	for(map<int, int>::iterator resIt = result.begin(); resIt != result.end(); resIt++)
	{
		cout << (*resIt).first << "\t" << (*resIt).second <<"\n";
	}
}

void DijkstraTest::runDijkstra(int startNode, map<int, map<int, int>> &graph, map<int, int> &distances)
{
	int currNode = startNode;
	bool finished   = false;
	set<int> visitedNodes;
	map<int, int> currNodeEdges;
	pair<int, int> minDist;

	while(!finished)
	{
		visitedNodes.insert(currNode);
		currNodeEdges = graph[currNode];

//		cout << "Current node: " << currNode << "\n";

		// Iterate over all edges emanating from current node
		for(map<int, int>::iterator currEdgeIt = currNodeEdges.begin(); currEdgeIt != currNodeEdges.end(); currEdgeIt++)
		{
//			cout << "Edge to " << (*currEdgeIt).first << " with weight " << (*currEdgeIt).second <<"\n";

			// Check if a path to the node pointed to by current edge already exists
			if(distances.find((*currEdgeIt).first) != distances.end())
			{
//				cout << "Path exists." << "Old weight: " << distances[(*currEdgeIt).first] << "\n";

				// If path via current node is shorter, update the distance
				if(((*currEdgeIt).second + distances[currNode]) < distances[(*currEdgeIt).first])
				{
					distances[(*currEdgeIt).first] = (*currEdgeIt).second + distances[currNode];
				}

//				cout << "New weight: " << (*currEdgeIt).second + distances[currNode] << "\n";
			}
			else
			{
				// Add new distance information for the node pointed to by current edge
				distances.insert(make_pair((*currEdgeIt).first, (*currEdgeIt).second + distances[currNode]));
//				cout << "Added new weight: " << (*currEdgeIt).second + distances[currNode] << "\n";
			}
		}
		// Find minimum distance from start node to not yet visited node
		minDist.second = INT_MAX;
		for(map<int, int>::iterator distIt = distances.begin(); distIt != distances.end(); distIt++)
		{
			if(visitedNodes.find((*distIt).first) == visitedNodes.end()) // Not yet in visited nodes
			{
				if((*distIt).second < minDist.second)
				{
					minDist = *distIt;
				}
			}
		}
		if(minDist.second == INT_MAX)
		{
			finished = true;
		}
		else
		{
			currNode = minDist.first;
		}
	}
}

DijkstraTest::~DijkstraTest()
{
	// TODO Auto-generated destructor stub
}

