#include "search.h"

void test_pq(std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open);

Search::Search(map<int, adjListNode*>& graphVertices, vector<pair<int,int> >& dotPositions, pair<int, int>& startPosition, int& mazeWidth, int& mazeHeight, 
	vector<vector<char> >& mazeText)
{
	this -> graphVertices = graphVertices;
	this -> dotPositions = dotPositions;
	this -> startPosition = startPosition;
	this -> mazeHeight = mazeHeight;
	this -> mazeWidth = mazeWidth;
	this -> mazeText = mazeText;
}


void Search::DFS_search()
{

	// Set up variables to use in DFS
	std::stack<int> frontier;

	bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited,false,this->mazeWidth * this->mazeHeight);

	// Get the linear idx of the start position
	int st_linear_idx = this->startPosition.second*this->mazeWidth + this->startPosition.first%this->mazeWidth;
	frontier.push(st_linear_idx);
	
	//** VERY LIKELY TO THROW UP ERROR **
	// adjListNode * head_curr_vertex = graphVertices[st_linear_idx];

	// if(head_curr_vertex == NULL)
	// 	cout << "the adj list is empty" << endl;
	
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	
	int steps_to_goal = -1;
	int nodes_expanded = 0;

	while(!frontier.empty())
	{
		steps_to_goal++;
		// if(steps_to_goal == 50)
			// break;
		// cout << "steps_to_goal: " << steps_to_goal << endl;
		int current_vertex = frontier.top();
		visited[current_vertex] = true;
		frontier.pop();
		adjListNode * head_curr_vertex = graphVertices[current_vertex];
		//vector<int> neighbors[NUM_NEIGHBORS];
		int curr_x = current_vertex% this->mazeWidth;
		int curr_y = current_vertex / this->mazeWidth;
		// cout << "curr_x: " << curr_x << "curr_y: " << curr_y << endl;
		if( (curr_x != this -> startPosition.first) || (curr_y != this->startPosition.second))
		{
			// cout << "I got to this point" << endl;
			mazeText[curr_y][curr_x] = 's';
		}
		if( (curr_x == dotx) && (curr_y == doty) )
		{
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			cout << "steps_to_goal: " << steps_to_goal << std::endl;
			for(int i = 0; i < mazeWidth*mazeHeight; i++)
			{
				if(i%mazeWidth == 0)
					std::cout << std::endl;
				std::cout << mazeText[i/mazeWidth][i%mazeWidth];
			}
			return;
		}

		while(head_curr_vertex != NULL)	
		{
			// **  NOTE: NEED A WAY TO DETECT ALREADY VISITED NODES
			if(!visited[head_curr_vertex->nodeId])
			{
				// cout << "got to this point in the inner while loop" << endl;
				frontier.push(head_curr_vertex->nodeId);
				//neighbors.push_back(head_curr_vertex->nodeId);
				head_curr_vertex = head_curr_vertex -> next;
			}
			else
			{
				head_curr_vertex = head_curr_vertex -> next;
 			}
 			nodes_expanded++;	
		}

	}
	
}

void Search::BFS_search()
{
	std::queue<int> frontier;

	bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited,false,this->mazeWidth * this->mazeHeight);

	int st_linear_idx = this->startPosition.second*this->mazeWidth + this->startPosition.first%this->mazeWidth;
	frontier.push(st_linear_idx);

	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	
	int steps_to_goal = -1;
	int nodes_expanded = 0;

	while(!frontier.empty())
	{
		steps_to_goal++;

		int current_vertex = frontier.front();
		visited[current_vertex] = true;
		frontier.pop();
		adjListNode * head_curr_vertex = graphVertices[current_vertex];

		int curr_x = current_vertex% this->mazeWidth;
		int curr_y = current_vertex / this->mazeWidth;

		if( (curr_x != this -> startPosition.first) || (curr_y != this->startPosition.second))
		{
			mazeText[curr_y][curr_x] = 's';
		}
		if( (curr_x == dotx) && (curr_y == doty) )
		{
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			cout << "steps_to_goal: " << steps_to_goal << std::endl;
			for(int i = 0; i < mazeWidth*mazeHeight; i++)
			{
				if(i % mazeWidth == 0)
					std::cout << std::endl;
				std::cout << mazeText[i / mazeWidth][i % mazeWidth];
			}
			std::cout << std::endl;
			return;
		}
		while(head_curr_vertex != NULL)	
		{
			if(!visited[head_curr_vertex->nodeId])
			{
				frontier.push(head_curr_vertex->nodeId);
				head_curr_vertex = head_curr_vertex -> next;
			}
			else
			{
				head_curr_vertex = head_curr_vertex -> next;
 			}
 			nodes_expanded++;	
		}

	}

}

void Search::greedy_search()
{
	//Create a priority queue
	std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open;

	// //Test function for the pq
	// test_pq(open);

	int st_linear_idx = this->startPosition.second*this->mazeWidth + this->startPosition.first%this->mazeWidth;
	open.push(graphVertices[st_linear_idx]);

	bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited,false,this->mazeWidth * this->mazeHeight);

	int steps_to_goal = -1;
	int nodes_expanded = 0;

	//While loop for greedy best-first search
	while(!open.empty())
	{
		steps_to_goal++;

		//Remove the node with the lowest mahattan distance
		adjListNode curr = open.top();
		open.pop();

		int curr_x = curr.nodeId%this->mazeWidth;
		int curr_y = curr.nodeId/this->mazeWidth;

		visited[curr.nodeId]= true;

		if( (curr_x != this -> startPosition.first) || (curr_y != this->startPosition.second))
		{
			mazeText[curr_y][curr_x] = 's';
		}

		if( (curr_x == dotx) && (curr_y == doty) )
		{
			cout << "Found the dot" << endl;
			for(int i = 0; i < mazeWidth*mazeHeight; i++)
			{
				if(i%mazeWidth == 0)
					std::cout << std::endl;
				std::cout << mazeText[i/mazeWidth][i%mazeWidth];
			}
			return;
		}

		adjListNode * neighbor_node = curr.next;

		while(neighbor_node != NULL)
		{
			if(!visited[neighbor_node->nodeId])
			{
				open.push(neighbor_node);
				neighbor_node = neighbor_node -> next;
			}
			else
			{
				neighbor_node = neighbor_node -> next;
 			}
 			nodes_expanded++;
		}

	}
}

float Search::mahattan_distance(pair<int,int> i_point, pair<int,int> f_point)
{
	return -1;
}


void test_pq(std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open)
{
	adjListNode toInsert[10];
	for(int i = 0; i < 10; i++)
	{
		toInsert[i].mahattan_distance = i + 10;
		open.push(toInsert[i]);
	}

	for(int j = 0; j < 10; j++)
	{
		adjListNode top = open.top();
		cout << top.mahattan_distance << endl;
		open.pop();
	}
}









