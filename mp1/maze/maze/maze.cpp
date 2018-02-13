#include "search.h"
using namespace std;

// parameter: filename of the .txt file for target maze
void initGraph(string fileName, map<int, adjListNode*>& graphVertices, vector<pair<int, int> >& dotPositions, pair<int, int>& startPosition, int& mazeWidth, int& mazeHeight,
	vector<vector<char> >& mazeText, vector<int>& dotIds) {

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

	// cout << "here?" << endl;
	// store maze width and height
	mazeWidth = line.size();
	mazeHeight = fileLines.size();
	// cout << "get the mazeWidth and mazeHeight?" <<endl;
	// store maze text into 2-D vector, indexing is column major
	// vector<vector<char> > mazeText;
	for (int i = 0; i < mazeHeight; i++) {
		vector<char> currLine;
		for (size_t j = 0; j < mazeWidth; j++) {
			currLine.push_back(fileLines[i].at(j));
		}
		mazeText.push_back(currLine);
	}

	vector<bool> visited;
	for (int i = 0; i < mazeHeight*mazeWidth; i++) {
		visited.push_back(false);
	}

	int currGrid = 0;

	for (int i = 0; i < mazeHeight; i++) {
		for (int j = 0; j < mazeWidth; j++) {
			if (mazeText[i][j] == '%') {
				continue;
			}

			adjListNode * curr;
			//check if node already in map
			if (graphVertices[i* mazeWidth + j] != NULL){
				
				curr = graphVertices[i*mazeWidth + j];
				curr->nodeId = i * mazeWidth + j;
				//cout << "present" << i << j;
			}
			else
			{
				curr = new adjListNode();
				curr->nodeId = i * mazeWidth + j;
				graphVertices[i*mazeWidth + j] = curr;

			}

			if (mazeText[i][j] == 'P') {
				pair<int, int> start(i, j);
				startPosition = start;
				curr->type = 1;
			}
			else if (mazeText[i][j] == '.') {
				pair<int, int> dot(i, j);
				dotPositions.push_back(dot);
				dotIds.push_back(i*mazeWidth + j);
				curr->type = 2;
			}
			else if (mazeText[i][j] == ' ')
			{
				curr->type = 3;
			}
			// check four neighbours of current grid
			//SINCE WE ARE GOING LEFT TO RIGHT, WE know right and bottom will be new nodes so need to add to map
			// and top and left will be in map

			//RIGHT
			if (mazeText[i][j + 1] != '%') {
				if (graphVertices[i* mazeWidth + j + 1] == NULL)
				{
					adjListNode *right = new adjListNode();
					right->nodeId = i * mazeWidth + j + 1;
					right->neighbours.push_back(curr);
					curr->neighbours.push_back(right);
					graphVertices[i*mazeWidth + j + 1] = right;
				}
				else
				{
					curr->neighbours.push_back(graphVertices[i* mazeWidth + j + 1]);
					graphVertices[i* mazeWidth + j + 1]->neighbours.push_back(curr);
				}
			}
			//DOWN
			if (mazeText[i + 1][j] != '%') {
				if (graphVertices[(i+1)* mazeWidth + j] == NULL)
				{
					adjListNode *down = new adjListNode();
					down->nodeId = (i + 1) * mazeWidth + j;
					down->neighbours.push_back(curr);
					curr->neighbours.push_back(down);
					graphVertices[(i + 1)*mazeWidth + j] = down;
				}
				else
				{
					curr->neighbours.push_back(graphVertices[(i + 1)* mazeWidth + j]);
					graphVertices[(i+1)* mazeWidth + j]->neighbours.push_back(curr);

				}
			}
			/*//LEFT
			if (mazeText[i][j - 1] != '%') {
				adjListNode *left = graphVertices[i*mazeWidth + j - 1];
				curr->neighbours.push_back(left);
			}
			//UP
			if (mazeText[i-1][j] != '%') {
				adjListNode *up = new adjListNode();
				up = graphVertices[(i-1)*mazeWidth + j];
				up->nodeId = (i-1)* mazeWidth + j;
				curr->neighbours.push_back(up);
			}*/

		}
	}

	/*for (auto elem : graphVertices)
	{
		std::cout << elem.first / mazeWidth << " " << elem.first%mazeWidth << " ";
		for (auto el2 : elem.second->neighbours)
			cout << el2->nodeId << " ";
		cout<<"\n";
	}*/
	//cout << size(dotPositions);
}



int main() {
	// testing graph construction with medium maze
	map<int, adjListNode*> graphVertices;
	vector<pair<int, int> > dotPositions;
	vector<int> dotIds;
	pair<int, int> startPosition;
	int mazeWidth, mazeHeight;
	vector<vector<char> > mazeText;
	initGraph("smallmulti.txt", graphVertices, dotPositions, startPosition, mazeWidth, mazeHeight, mazeText,dotIds);
	//cout << startPosition.first%mazeWidth + startPosition.second*mazeWidth << endl;

	//Declare a search data structure that can execute all given searches
	Search new_Search(graphVertices, dotPositions, startPosition, mazeWidth, mazeHeight, mazeText, dotIds);

	if (dotPositions.size() < 2)
	{
		// Here we execute the 1.1 search algorithms.
		// cout << "is it in the search or befo?" << endl;

		//new_Search.reset_graph();
		new_Search.DFS_search(); // Execute DFS search
		
		new_Search.reset_graph();
		new_Search.BFS_search();

		new_Search.reset_graph();
		new_Search.greedy_search();
		
		new_Search.reset_graph();
		new_Search.astar_search();
		int wait;
		cin >> wait;

	}
	else
	{
		new_Search.multi_search();
		int wait;
		cin >> wait;
	}
	return 0;
}
