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
		if(steps_to_goal == 1)
			break;
		// cout << "steps_to_goal: " << steps_to_goal << endl;
		int current_vertex = frontier.top();
		visited[current_vertex] = true;
		frontier.pop();
		adjListNode * head_curr_vertex = graphVertices[current_vertex];
		cout << "current_vertex: " << current_vertex << endl; 
		cout << "adjListNode * head_curr_vertex = graphVertices[current_vertex];" << endl;
		cout << "head_curr_vertex -> nodeId: " << head_curr_vertex -> nodeId << endl;
		//vector<int> neighbors[NUM_NEIGHBORS];
		int curr_x = current_vertex% this->mazeWidth;
		int curr_y = current_vertex / this->mazeWidth;
		// cout << "curr_x: " << curr_x << "curr_y: " << curr_y << endl;
		if( (curr_x != this -> startPosition.first) || (curr_y != this->startPosition.second))
		{
			// cout << "I got to this point" << endl;
			mazeText[curr_y][curr_x] = '.';
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
				cout << "moving to the next node" << endl;
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
			mazeText[curr_y][curr_x] = '.';
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
	open.push(*(graphVertices[st_linear_idx]));

	bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited,false,this->mazeWidth * this->mazeHeight);

	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;

	int steps_to_goal = -1;
	int nodes_expanded = 0;

	//While loop for greedy best-first search
	while(!open.empty())
	{
		steps_to_goal++;
		// if(steps_to_goal == 150)
		// {
		// 	break;
		// }
		cout << "Steps to goal: " << steps_to_goal << endl;

		//Remove the node with the lowest mahattan distance
		adjListNode curr = open.top();
		open.pop();
		// cout << "curr.nodeId: " << curr.nodeId << endl;
		// while(!open.empty())
		// {
		// 	open.pop();
		// }

		int curr_x = curr.nodeId%this->mazeWidth;
		int curr_y = curr.nodeId/this->mazeWidth;

		visited[curr.nodeId]= true;

		if( (curr_x == dotx) && (curr_y == doty) )
		{
			cout << "Found the dot" << endl;
			for(int i = 0; i < mazeWidth*mazeHeight; i++)
			{
				if(i%mazeWidth == 0)
					std::cout << std::endl;
				std::cout << mazeText[i/mazeWidth][i%mazeWidth];
			}
			std::cout << std::endl;
			return;
		}

		if( (curr_x != this -> startPosition.first) || (curr_y != this->startPosition.second))
		{
			mazeText[curr_y][curr_x] = '.';
		}


		adjListNode * neighbor_node = graphVertices[curr.nodeId];
		if(neighbor_node == NULL)
			cout << "The neighbor_node is NULL" << endl;

		while(neighbor_node != NULL)
		{
			if(!visited[neighbor_node->nodeId])
			{
				int x = (neighbor_node -> nodeId)%mazeWidth;
				int y = (neighbor_node -> nodeId)/mazeWidth;
				pair<int,int> i_point(x,y);
				pair<int,int> f_point(dotx,doty);

				neighbor_node -> h_distance = mahattan_distance(i_point,f_point);
				open.push(*neighbor_node);
				neighbor_node = neighbor_node -> next;
			}
			else
			{
				neighbor_node = neighbor_node -> next;
 			}
 			nodes_expanded++;
		}

	}
	for(int i = 0; i < mazeWidth*mazeHeight; i++)
	{
		if(i%mazeWidth == 0)
			std::cout << std::endl;
		std::cout << mazeText[i/mazeWidth][i%mazeWidth];
	}
	std::cout << std::endl;
}

void Search::astar_search()
{
	//Create a priority queue
	std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open;

	// //Test function for the pq
	// test_pq(open);

	int st_linear_idx = this->startPosition.second*this->mazeWidth + this->startPosition.first%this->mazeWidth;
	open.push(*(graphVertices[st_linear_idx]));

	bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited,false,this->mazeWidth * this->mazeHeight);

	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;

	int steps_to_goal = -1;
	int nodes_expanded = 0;

	//While loop for greedy best-first search
	while(!open.empty())
	{
		steps_to_goal++;
		// if(steps_to_goal == 150)
		// {
		// 	break;
		// }
		cout << "Steps to goal: " << steps_to_goal << endl;

		//Remove the node with the lowest mahattan distance
		adjListNode curr = open.top();
		open.pop();
		// cout << "curr.nodeId: " << curr.nodeId << endl;
		// while(!open.empty())
		// {
		// 	open.pop();
		// }

		int curr_x = curr.nodeId%this->mazeWidth;
		int curr_y = curr.nodeId/this->mazeWidth;

		visited[curr.nodeId]= true;

		if( (curr_x == dotx) && (curr_y == doty) )
		{
			cout << "Found the dot" << endl;
			for(int i = 0; i < mazeWidth*mazeHeight; i++)
			{
				if(i%mazeWidth == 0)
					std::cout << std::endl;
				std::cout << mazeText[i/mazeWidth][i%mazeWidth];
			}
			std::cout << std::endl;
			return;
		}

		if( (curr_x != this -> startPosition.first) || (curr_y != this->startPosition.second))
		{
			mazeText[curr_y][curr_x] = '.';
		}


		adjListNode * neighbor_node = graphVertices[curr.nodeId];
		if(neighbor_node == NULL)
			cout << "The neighbor_node is NULL" << endl;

		while(neighbor_node != NULL)
		{
			if(!visited[neighbor_node->nodeId])
			{
				int x = (neighbor_node -> nodeId)%mazeWidth;
				int y = (neighbor_node -> nodeId)/mazeWidth;
				pair<int,int> i_point(x,y);
				pair<int,int> f_point(dotx,doty);

				neighbor_node -> h_distance = mahattan_distance(i_point,f_point) + steps_to_goal;
				open.push(*neighbor_node);
				neighbor_node = neighbor_node -> next;
			}
			else
			{
				neighbor_node = neighbor_node -> next;
 			}
 			nodes_expanded++;
		}

	}
	for(int i = 0; i < mazeWidth*mazeHeight; i++)
	{
		if(i%mazeWidth == 0)
			std::cout << std::endl;
		std::cout << mazeText[i/mazeWidth][i%mazeWidth];
	}
	std::cout << std::endl;
}

float Search::mahattan_distance(pair<int,int> i_point, pair<int,int> f_point)
{
	return std::abs((i_point.first - f_point.first) + (i_point.second - f_point.second));
}

float Search::start_distance(pair<int,int> i_point, pair<int,int> f_point)
{
	return 0;
}

void test_pq(std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open)
{
	adjListNode toInsert[10];
	for(int i = 0; i < 10; i++)
	{
		toInsert[i].h_distance = i + 10;
		open.push(toInsert[i]);
	}

	for(int j = 0; j < 10; j++)
	{
		adjListNode top = open.top();
		cout << top.h_distance << endl;
		open.pop();
	}
}









