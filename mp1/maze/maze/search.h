#include <vector>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <cmath>
#define NUM_NEIGHBORS 4
using namespace std;


typedef struct adjListNode{
	// node id is the index of the node in the flattend maze matrix (y * width + x)
	float h_distance;
	int nodeId;
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


class Search{
	public:
		Search(map<int, adjListNode*>& graphVertices, vector<pair<int,int> >& dotPositions, pair<int, int>& startPosition, int& mazeWidth, int& mazeHeight,
			vector<vector<char> >& mazeText);
		void DFS_search();
		void BFS_search();
		void greedy_search();
		void astar_search();

		//Helper function for calculating the Mahattan distance between two points
		float mahattan_distance(pair<int,int> i_point, pair<int,int> f_point);
		float start_distance(pair<int,int> i_point, pair<int,int> f_point);
		void print_maze();
		void reset_graph();

	private:
		//Variables that store the maze representation
		map<int, adjListNode*> graphVertices;
		vector<pair<int,int> > dotPositions;
		pair<int, int> startPosition;
		int mazeWidth, mazeHeight;
		vector<vector<char> > mazeText;

};










