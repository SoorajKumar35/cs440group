#pragma once
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include <vector>
#include <stack>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <cmath>
#include <functional>
#define NUM_NEIGHBORS 4
using namespace std;


typedef struct adjListNode {
	// node id is the index of the node in the flattend maze matrix (y * width + x)
	float h_distance;
	int nodeId=0;
	int type=0;  // 1 is P, 2 is '.', 3 is ' '
	vector <adjListNode *> neighbours;  //all free spaces around a node
	struct adjListNode* next;
	bool operator<(struct adjListNode const& b) const
	{
		return h_distance < b.h_distance;
	}
	bool operator>(struct adjListNode const& b) const
	{
		return h_distance > b.h_distance;
	}
	bool operator==(struct adjListNode const& b) const
	{
		return nodeId == b.nodeId;
	}

	int steps_from_start;
} adjListNode;


class Search {
public:
	Search(map<int, adjListNode*>& graphVertices, vector<pair<int, int> >& dotPositions, pair<int, int>& startPosition, int& mazeWidth, int& mazeHeight,
		vector<vector<char> >& mazeText, vector<int> dotIds);
	void DFS_search();
	void DFS_recurse(int start,adjListNode * v, bool visited[],int finald, int pathc);

	void BFS_search();
	void greedy_search();
	void astar_search();


	int findNearestDot(vector<pair<int, int>> dots, vector<bool> dotsVisited, int currX, int currY);
	void suboptimal_search();


	//Helper function for calculating the Mahattan distance between two points
	float mahattan_distance(pair<int, int> i_point, pair<int, int> f_point);
	float start_distance(pair<int, int> i_point, pair<int, int> f_point);
	void print_maze();
	void reset_graph();
	void multi_search();
	int dist_dots(int agentId, vector<int>dots);
private:
	//Variables that store the maze representation
	map<int, adjListNode*> graphVertices;
	vector<pair<int, int> > dotPositions;
	vector<int> dotIds;
	pair<int, int> startPosition;
	int mazeWidth, mazeHeight;
	int numgoals;
	vector<vector<char> > mazeText;

};
