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
	std::map<int,int> parents;

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
		// if(steps_to_goal == 1)
		// 	break;
		//cout << "steps_to_goal: " << steps_to_goal << endl;
		int current_vertex = frontier.top();
		visited[current_vertex] = true;
		frontier.pop();
		adjListNode * head_curr_vertex = graphVertices[current_vertex];
		// cout << "current_vertex: " << current_vertex << endl; 
		// cout << "adjListNode * head_curr_vertex = graphVertices[current_vertex];" << endl;
		// cout << "head_curr_vertex -> nodeId: " << head_curr_vertex -> nodeId << endl;
		// vector<int> neighbors[NUM_NEIGHBORS];
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
			int current_parent = parents[current_vertex];
			int path_cost = 0;
			while(current_parent != st_linear_idx)
			{
				path_cost++;
				current_parent = parents[current_parent];
			}
			print_maze();
			cout << "DFS_search" << endl;
			cout << "Path cost: " << path_cost << endl;
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			cout << "steps_to_goal: " << steps_to_goal << std::endl;
			
		}

		while(head_curr_vertex != NULL)	
		{
			// **  NOTE: NEED A WAY TO DETECT ALREADY VISITED NODES
			if(!visited[head_curr_vertex->nodeId])
			{
				// cout << "got to this point in the inner while loop" << endl;
				parents[head_curr_vertex->nodeId] = current_vertex;
				frontier.push(head_curr_vertex->nodeId);
				//neighbors.push_back(head_curr_vertex->nodeId);
				head_curr_vertex = head_curr_vertex -> next;
			}
			else
			{
				//cout << "moving to the next node" << endl;
				head_curr_vertex = head_curr_vertex -> next;
 			}
 			nodes_expanded++;	
		}

	}
	
}

void Search::BFS_search()
{
	std::queue<int> frontier;
	std::map<int, int> parents;
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
			int current_parent = parents[current_vertex];
			int path_cost = 0;
			while(current_parent != st_linear_idx)
			{
				path_cost++;
				current_parent = parents[current_parent];
			}
			print_maze();
			cout << "BFS_search" << endl;
			std::cout << "Found the dot" << std::endl;
			cout << "Path cost: " << path_cost << endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			cout << "steps_to_goal: " << steps_to_goal << std::endl;
			
			return;
		}
		while(head_curr_vertex != NULL)	
		{
			if(!visited[head_curr_vertex->nodeId])
			{
				parents[head_curr_vertex->nodeId] = current_vertex;
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
	std::map<int,int> parents;
	// //Test function for the pq
	// test_pq(open);

	int st_linear_idx = this->startPosition.second*this->mazeWidth + this->startPosition.first%this->mazeWidth;
	// open.push(*(graphVertices[st_linear_idx]));

	bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited,false,this->mazeWidth * this->mazeHeight);

	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;

	int steps_to_goal = -1;
	int nodes_expanded = 0;

	adjListNode * start_n = graphVertices[st_linear_idx];
	while(start_n != NULL)
	{
		if(!visited[start_n->nodeId])
		{
			int x = (start_n -> nodeId)%mazeWidth;
			int y = (start_n -> nodeId)/mazeWidth;
			pair<int,int> i_point(x,y);
			pair<int,int> f_point(dotx,doty);
			start_n -> h_distance = mahattan_distance(i_point,f_point);
			parents[start_n->nodeId] = st_linear_idx;
			open.push(*start_n);
			start_n = start_n -> next;
		}
		else
		{
			start_n = start_n -> next;
 		}
	}


	//While loop for greedy best-first search
	while(!open.empty())
	{
		steps_to_goal++;
		// if(steps_to_goal == 150)
		// {
		// 	break;
		// }
		//cout << "Steps to goal: " << steps_to_goal << endl;

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
		    int path_cost = 0;	
			int current_parent = parents[curr.nodeId];
			while(current_parent != st_linear_idx)
			{
				path_cost++;
				current_parent = parents[current_parent];
			}
			print_maze();
			cout << "Greedy_search" << endl;
			cout << "path_cost: " << path_cost << endl;
			cout << "steps_to_goal: " << steps_to_goal << endl;
			cout << "nodes_expanded " << nodes_expanded << endl;
			cout << "Found the dot" << endl;
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

				parents[neighbor_node->nodeId] = curr.nodeId;
				
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
	print_maze();
}

void Search::astar_search()
{
	//Create a priority queue
	std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open;
	std::map<int,int> parents;

	//Test function for the pq
	// test_pq(open);

    // cout << "Start_x: " << startPosition.first << " Start_y: " << startPosition.second << endl;
    // cout << "end_x: " << dotPositions[0].first << " end_y: " << dotPositions[0].second << endl;
    // cout << "mazeWidth: " << mazeWidth << " mazeHeight: " << mazeHeight << endl;
	int st_linear_idx = this->startPosition.second*this->mazeWidth + this->startPosition.first%this->mazeWidth;
	// cout << "st_linear_idx: " << st_linear_idx << endl;
	//graphVertices[st_linear_idx]->steps_from_start = 1;
	//open.push(*(graphVertices[st_linear_idx]));

	bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited,false,this->mazeWidth * this->mazeHeight);
    visited[st_linear_idx] = true;

	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;

	int steps_to_goal = -1;
	int nodes_expanded = 0;

	adjListNode start_node;
	start_node.nodeId = st_linear_idx;

	adjListNode * neighbor_node;
	neighbor_node = graphVertices[st_linear_idx];

	while(neighbor_node != NULL)
	{
		if(!visited[neighbor_node -> nodeId])
		{
			int x = (neighbor_node -> nodeId)%mazeWidth;
			int y = (neighbor_node -> nodeId)/mazeWidth;
			// cout << "neighbor_node_x: " << x << " neighbor_node_y: " << y << endl;
			pair<int,int> i_point(x,y);
			pair<int,int> f_point(dotx,doty);
			neighbor_node -> steps_from_start = 1;
			neighbor_node -> h_distance = mahattan_distance(i_point,f_point) + neighbor_node->steps_from_start;
			// cout << "m_distance :" << mahattan_distance(i_point,f_point)  << endl;
			// cout << "h_distance :" << mahattan_distance(i_point,f_point) + neighbor_node->steps_from_start << endl;
			parents[neighbor_node -> nodeId] = st_linear_idx;
			open.push(*neighbor_node);
			neighbor_node = neighbor_node -> next;
		}
		else
			neighbor_node = neighbor_node -> next;
		//nodes_expanded++;
	}
	//std::cout << endl;
	//cout << "nodes_expanded: " << nodes_expanded << endl;
	//While loop for greedy best-first search
	while(!open.empty())
	{
		nodes_expanded++;		
	    
		//cout << "Steps to goal: " << steps_to_goal << endl;
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
			// Get parent of the goal node


			int current_parent = parents[curr.nodeId];
			cout << "curr.nodeId = " << curr.nodeId << endl;
			cout << "current_parent.nodeId = " << current_parent << endl;
			cout << "Parent of current_parent.nodeId = " << parents[current_parent] << endl; 
			int path_cost = 0;
			while(current_parent != st_linear_idx)
			{
				path_cost++;
				current_parent = parents[current_parent];
			}
			print_maze();
			cout << "astar_search" << endl;
			cout << "path_cost: " << path_cost << endl;
			cout << "steps_to_goal: " << steps_to_goal << endl;
			cout << "nodes_expanded " << nodes_expanded << endl;
			cout << "Found the dot" << endl;
			return;
		}



		if( (curr_x != this -> startPosition.first) || (curr_y != this->startPosition.second))
		{
			// int reduced = curr.steps_from_start%10;
			// pair<int,int> i_point(curr_x,curr_y);
			// pair<int,int> f_point(dotx,doty);
			// int m_d = mahattan_distance(i_point,f_point);
			// m_d %= 10;
			mazeText[curr_y][curr_x] = '.';//('0' + m_d);
		}


		neighbor_node = graphVertices[curr.nodeId];

		if(neighbor_node == NULL)
			cout << "The neighbor_node is NULL" << endl;
        
   
		while(neighbor_node != NULL)
		{
			if(!visited[neighbor_node->nodeId])
			{
				int x = (neighbor_node -> nodeId)%mazeWidth;
				int y = (neighbor_node -> nodeId)/mazeWidth;
				//cout << "neighbor_node_x: " << x << " neighbor_node_y: " << y << endl;


				pair<int,int> i_point(x,y);
				pair<int,int> f_point(dotx,doty);
				neighbor_node -> steps_from_start = curr.steps_from_start + 1; 
				neighbor_node -> h_distance = mahattan_distance(i_point,f_point) + neighbor_node->steps_from_start;


				parents[neighbor_node -> nodeId] = curr.nodeId;
				//cout << "m_distance :" << mahattan_distance(i_point,f_point) << endl;
				//cout << "h_distance :" << mahattan_distance(i_point,f_point) + neighbor_node->steps_from_start << endl;


				open.push(*neighbor_node);
				neighbor_node = neighbor_node -> next;
			}
			else
			{
				neighbor_node = neighbor_node -> next;
 			}
 			nodes_expanded++;
		}
		//std::cout<<endl;

	}

	

	print_maze();
}

void Search::print_maze()
{
	for(int i = 0; i < mazeWidth*mazeHeight; i++)
	{
		if(i%mazeWidth == 0)
			std::cout << std::endl;
		std::cout << mazeText[i/mazeWidth][i%mazeWidth];
	}
	std::cout << std::endl;
}

void Search::reset_graph()
{
	for(int i = 0; i < mazeWidth*mazeHeight; i++)
	{
		if(mazeText[i/mazeWidth][i%mazeWidth] == '.')
		{
			mazeText[i/mazeWidth][i%mazeWidth] = ' ';
		}
	}
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	mazeText[doty][dotx] = '.';
}

float Search::mahattan_distance(pair<int,int> i_point, pair<int,int> f_point)
{
	return std::abs((i_point.first - f_point.first)) + std::abs((i_point.second - f_point.second));
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









