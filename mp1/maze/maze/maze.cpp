#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>

using namespace std;


struct adjListNode{
	// node id is the index of the node in the flattend maze matrix (y * width + x)
	int nodeId;
	struct adjListNode* next;
};



// parameter: filename of the .txt file for target maze
void initGraph(string fileName, map<int, adjListNode*>& graphVertices, vector<pair<int,int>>& dotPositions, pair<int, int>& startPosition, int& mazeWidth, int& mazeHeight){

	// read from maze text file line by line
	vector<string> fileLines;
	string line;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			fileLines.push_back(line);
		}
		myfile.close();
	}

	// store maze width and height
	mazeWidth = line.size();
	mazeHeight = fileLines.size();

	// store maze text into 2-D vector, indexing is column major
	vector<vector<char>> mazeText;
	for (int i = 0; i < mazeHeight; i++){
		vector<char> currLine;
		for (size_t j = 0; j < mazeWidth; j++){
			currLine.push_back(fileLines[i].at(j));
		}
		mazeText.push_back(currLine);
	}

	vector<bool> visited;
	for (int i = 0; i < mazeHeight*mazeWidth; i++){
		visited.push_back(false);
	}

	
	// find the first empty grid in the maze to start graph traversal
	int currGrid = 0;
	while (mazeText[currGrid / mazeWidth][currGrid%mazeWidth] == '%'){
		currGrid++;
	}
	int curr_x = currGrid%mazeWidth;
	int curr_y = currGrid/mazeWidth;
	
	queue<int> q;
	q.push(currGrid);
	visited[currGrid] = true;

	while (!q.empty()){
		currGrid = q.front();
		curr_x = currGrid%mazeWidth;
		curr_y = currGrid / mazeWidth;
		//visited[currGrid] = true;

		// store its coordinate if the current node is a starting point or a dot
		if (mazeText[curr_y][curr_x] == 'P'){
			pair<int, int> start(curr_x, curr_y);
			startPosition = start;
		}
		else if(mazeText[curr_y][curr_x] == '.'){
			pair<int, int> dot(curr_x, curr_y);
			dotPositions.push_back(dot);
		}

		// initialize adj list for current node
		adjListNode* listHead = new adjListNode();
		adjListNode* curr = listHead;

		// check four neighbours of current grid
		if (curr_x < mazeWidth - 1 && mazeText[curr_y][curr_x + 1] != '%'){
			curr->next = new adjListNode();
			curr = curr->next;
			curr->nodeId = currGrid + 1;
			if (!visited[currGrid + 1]){
				visited[currGrid + 1] = true;
				q.push(currGrid + 1);
			}
		}

		if (curr_x > 0 && mazeText[curr_y][curr_x - 1] != '%'){
			curr->next = new adjListNode();
			curr = curr->next;
			curr->nodeId = currGrid - 1;
			if (!visited[currGrid - 1]){
				visited[currGrid - 1] = true;
				q.push(currGrid - 1);
			}
		}

		if (curr_y > 0 && mazeText[curr_y - 1][curr_x] != '%'){
			curr->next = new adjListNode();
			curr = curr->next;
			curr->nodeId = currGrid - mazeWidth;
			if (!visited[currGrid - mazeWidth]){
				visited[currGrid - mazeWidth] = true;
				q.push(currGrid - mazeWidth);
			}
		}
		if (curr_y < mazeHeight - 1 && mazeText[curr_y + 1][curr_x] != '%'){
			curr->next = new adjListNode();
			curr = curr->next;
			curr->nodeId = currGrid + mazeWidth;
			if (!visited[currGrid + mazeWidth]){
				visited[currGrid + mazeWidth] = true;
				q.push(currGrid + mazeWidth);
			}
		}

		// update adjlist of current node
		graphVertices[currGrid] = listHead->next;

		// pop processed node
		q.pop();
	}
}

int main(){
	// testing graph construction with medium maze
	map<int, adjListNode*> graphVertices;
	vector<pair<int,int>> dotPositions;
	pair<int, int> startPosition;
	int mazeWidth, mazeHeight;
	initGraph("mediumMaze.txt", graphVertices, dotPositions, startPosition, mazeWidth, mazeHeight);
	return 0;
}