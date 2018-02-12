#include "search.h"

void test_pq(std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open);

Search::Search(map<int, adjListNode*>& graphVertices, vector<pair<int, int> >& dotPositions, pair<int, int>& startPosition, int& mazeWidth, int& mazeHeight,
	vector<vector<char> >& mazeText, vector<int> dotIds)
{
	this->graphVertices = graphVertices;
	this->dotPositions = dotPositions;
	this->startPosition = startPosition;
	this->mazeHeight = mazeHeight;
	this->mazeWidth = mazeWidth;
	this->mazeText = mazeText;
	this->dotIds = dotIds;
}
int nodes_expanded=0;
void Search::DFS_recurse(int start,adjListNode * v,bool visited[], int finald, int pathc)
{
	if ((v)->nodeId == finald)
	{
		
		cout << "DFS_search" << endl;
		cout << "Path cost: " << pathc-1 << endl;
		std::cout << "Found the dot" << std::endl;
		cout << "nodes_expanded: " << nodes_expanded << std::endl;
		print_maze();
		return;
	}
	visited[v->nodeId] = true;
	vector<adjListNode *> ::iterator i;
	/*cout <<"\n"<<v->nodeId;
	for (i = v->neighbours.begin(); i != v->neighbours.end(); ++i)
		cout << (*i)->nodeId<<" ";*/
	for (i = v->neighbours.begin(); i != v->neighbours.end(); ++i)
	{
		if(v->nodeId != start && v->nodeId != finald)
			mazeText[v->nodeId / mazeWidth][v->nodeId%mazeWidth] = '.';
		//NICE DFS ANIMATION
		//print_maze();
		//std::this_thread::sleep_for(std::chrono::milliseconds(300));
		if (!visited[(*i)->nodeId])
		{
			nodes_expanded++;
			DFS_recurse(start,*i, visited, finald, ++pathc);
		}
		if (v->nodeId != start)
			mazeText[v->nodeId / mazeWidth][v->nodeId%mazeWidth] = ' ';

	}
}

void Search::DFS_search()
{
	nodes_expanded = 0;

	// Set up variables to use in DFS
	bool * visited = new bool[this->mazeWidth * this->mazeHeight];
	std::memset(visited, false, this->mazeWidth * this->mazeHeight);
	// Get the linear idx of the start position
	int st_linear_idx = this->startPosition.first*this->mazeWidth + this->startPosition.second;
	
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	
	int final_dot = dotx * this->mazeWidth + doty;
	int nodes_expanded = 0;
	int path_cost=0;
	DFS_recurse(st_linear_idx,graphVertices[st_linear_idx], visited,final_dot, path_cost);

}


void Search::BFS_search()
{
	nodes_expanded = 0;
	std::queue<adjListNode *> frontier;
	std::queue<int> frontiercosts;
	std::queue<vector<int>> paths;

	std::map<int, int> parents;
	bool * visited = new bool[this->mazeWidth * this->mazeHeight];

	//bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited, false, this->mazeWidth * this->mazeHeight);

	int st_linear_idx = this->startPosition.first*this->mazeWidth + this->startPosition.second;
	frontier.push(graphVertices[st_linear_idx]);
	frontiercosts.push(0);
	visited[st_linear_idx] = true;
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	int finald = dotx * mazeWidth + doty;
	int nodes_expanded = 0;
	int current_vertex;
	int pathc = 0;
	vector<int> finalpath;
	finalpath.push_back(st_linear_idx);
	paths.push(finalpath);

	while (!frontier.empty())
	{

		adjListNode * v = frontier.front();
		pathc = frontiercosts.front();
		finalpath = paths.front();

		/*NICE BFS ANIMATION
		if (v->nodeId != st_linear_idx && v->nodeId != finald)
			mazeText[v->nodeId / mazeWidth][v->nodeId%mazeWidth] = '.';
		print_maze();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));*/
		current_vertex = v->nodeId;
		if (current_vertex == finald)
		{
			cout << "BFS_search" << endl;
			cout << "Path cost: " << pathc-1 << endl;
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			break;
		}		
		frontier.pop();
		frontiercosts.pop();
		paths.pop();
		vector<adjListNode *> ::iterator i;

		for (i = v->neighbours.begin(); i != v->neighbours.end(); ++i)
		{
			int temp;
			vector<int> temppath;
			if (!visited[(*i)->nodeId])
			{
				nodes_expanded++;
				visited[(*i)->nodeId] = true;
				frontier.push(*i);
				temp = pathc;
				frontiercosts.push(temp+1);
				temppath = finalpath;
				temppath.push_back((*i)->nodeId);
				paths.push(temppath);
			}
		}
	}
	vector<int> ::iterator j;

	for (j = finalpath.begin()+1; j!= finalpath.end();++j)
		mazeText[*j/ mazeWidth][*j%mazeWidth] = '.';
	print_maze();

}


void Search::greedy_search()
{
	nodes_expanded = 0;
	std::vector<adjListNode *> frontier;
	std::vector<int> frontiercosts;
	std::vector<vector<int>> paths;
	std::vector<int> h_queue;
	std::map<int, int> parents;
	bool * visited = new bool[this->mazeWidth * this->mazeHeight];
	int h_distance;
	//bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited, false, this->mazeWidth * this->mazeHeight);

	int st_linear_idx = this->startPosition.first*this->mazeWidth + this->startPosition.second;
	frontier.push_back(graphVertices[st_linear_idx]);
	frontiercosts.push_back(0);

	visited[st_linear_idx] = true;
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	int finald = dotx * mazeWidth + doty;
	int nodes_expanded = 0;
	int current_vertex;
	int pathc = 0;

	pair<int, int> i_point(startPosition.first, startPosition.second);
	pair<int, int> f_point(dotx, doty);
	h_distance = mahattan_distance(i_point, f_point);
	h_queue.push_back(h_distance);

	vector<int> finalpath;
	finalpath.push_back(st_linear_idx);
	paths.push_back(finalpath);

	while (!frontier.empty())
	{

		adjListNode * v = frontier.front();
		pathc = frontiercosts.front();
		finalpath = paths.front();

		/*NICE GBFS ANIMATION
		if (v->nodeId != st_linear_idx && v->nodeId != finald)
		mazeText[v->nodeId / mazeWidth][v->nodeId%mazeWidth] = '.';
		print_maze();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));*/
		current_vertex = v->nodeId;
		if (current_vertex == finald)
		{
			cout << "GBFS_search" << endl;
			cout << "Path cost: " << pathc-1 << endl;
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			break;
		}
		frontier.erase(frontier.begin());
		frontiercosts.erase(frontiercosts.begin());
		paths.erase(paths.begin());
		h_queue.erase(h_queue.begin());
		vector<adjListNode *> ::iterator i;

		for (i = v->neighbours.begin(); i != v->neighbours.end(); ++i)
		{
			int temp;
			vector<int> temppath;
			if (!visited[(*i)->nodeId])
			{

				nodes_expanded++;
				visited[(*i)->nodeId] = true;
				int index_pri = 0;
				pair<int, int> i_point((*i)->nodeId/mazeWidth, (*i)->nodeId %mazeWidth);
				pair<int, int> f_point(dotx, doty);
				h_distance = mahattan_distance(i_point, f_point);
				auto ir=h_queue.begin();
				if (!h_queue.empty())
				{
					if (h_distance < *ir)
					{
						index_pri = 0;
						
					}
					else {
						//FIND WHERE TO INSERT
						for (ir = h_queue.begin(); ir != h_queue.end(); ++ir)
							if (h_distance < *ir)
							{
								break;
							}
						index_pri = ir - h_queue.begin();
					}
				}
				//IF FOR PUSH AT END(INDEX ERRORS) OR PUSH IN MIDDLE
				if (h_queue.empty() || index_pri==h_queue.size())
				{
					//cout << "here";
					h_queue.push_back(h_distance);
					frontier.push_back(*i);
					temp = pathc;
					frontiercosts.push_back(temp + 1);
					temppath = finalpath;
					temppath.push_back((*i)->nodeId);
					paths.push_back(temppath);
				}
				else {
					
					h_queue.insert(h_queue.begin() + index_pri, h_distance);
					frontier.insert(frontier.begin()+index_pri, *i);
					temp = pathc;
					frontiercosts.insert(frontiercosts.begin()+ index_pri, temp + 1);
					temppath = finalpath;
					temppath.push_back((*i)->nodeId);
					paths.insert(paths.begin()+index_pri,temppath);


				}
	
			}
		}
	}
	vector<int> ::iterator j;

	for (j = finalpath.begin()+1; j != finalpath.end();++j)
		mazeText[*j / mazeWidth][*j%mazeWidth] = '.';
	print_maze();
}



void Search::astar_search()
{
	nodes_expanded = 0;
	std::vector<adjListNode *> frontier;
	std::vector<int> frontiercosts;
	std::vector<vector<int>> paths;
	std::vector<int> f_queue;
	std::map<int, int> parents;
	bool * visited = new bool[this->mazeWidth * this->mazeHeight];
	int h_distance;
	int dist_from_start;
	//bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited, false, this->mazeWidth * this->mazeHeight);

	int st_linear_idx = this->startPosition.first*this->mazeWidth + this->startPosition.second;
	frontier.push_back(graphVertices[st_linear_idx]);
	frontiercosts.push_back(0);

	visited[st_linear_idx] = true;
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	int finald = dotx * mazeWidth + doty;
	int nodes_expanded = 0;
	int current_vertex;
	int pathc = 0;
	int f_dist;
	pair<int, int> st_point(startPosition.first, startPosition.second);
	pair<int, int> f_point(dotx, doty);
	h_distance = mahattan_distance(st_point, f_point);
	dist_from_start = 0;
	f_queue.push_back(h_distance);
	vector<int> finalpath;
	finalpath.push_back(st_linear_idx);
	paths.push_back(finalpath);

	while (!frontier.empty())
	{

		adjListNode * v = frontier.front();
		pathc = frontiercosts.front();
		finalpath = paths.front();

		//NICE ASTAR ANIMATION
		/*if (v->nodeId != st_linear_idx && v->nodeId != finald)
		mazeText[v->nodeId / mazeWidth][v->nodeId%mazeWidth] = '.';
		print_maze();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));*/

		current_vertex = v->nodeId;
		if (current_vertex == finald)
		{
			cout << "ASTAR_search" << endl;
			cout << "Path cost: " << pathc-1 << endl;
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			break;
		}
		frontier.erase(frontier.begin());
		frontiercosts.erase(frontiercosts.begin());
		paths.erase(paths.begin());
		f_queue.erase(f_queue.begin());
		vector<adjListNode *> ::iterator i;

		for (i = v->neighbours.begin(); i != v->neighbours.end(); ++i)
		{
			int temp;
			vector<int> temppath;
			if (!visited[(*i)->nodeId])
			{

				nodes_expanded++;
				visited[(*i)->nodeId] = true;
				int index_pri = 0;
				pair<int, int> i_point((*i)->nodeId / mazeWidth, (*i)->nodeId %mazeWidth);
				pair<int, int> f_point(dotx, doty);
				h_distance = mahattan_distance(i_point, f_point);
				dist_from_start = mahattan_distance(st_point, i_point);
				f_dist = h_distance + dist_from_start;

				auto ir = f_queue.begin();
				//F_QUEUE HOLDS THE F DISTANCES AND SORTS THEM
				if (!f_queue.empty())
				{
					if (f_dist < *ir)
					{
						index_pri = 0;

					}
					else {
						//FIND WHERE TO INSERT
						for (ir = f_queue.begin(); ir != f_queue.end(); ++ir)
							if (f_dist < *ir)
							{
								break;
							}
						index_pri = ir - f_queue.begin();
					}
				}
				//IF FOR PUSH AT END(INDEX ERRORS) OR PUSH IN MIDDLE
				if (f_queue.empty() || index_pri == f_queue.size())
				{
					//cout << "here";
					f_queue.push_back(f_dist);
					frontier.push_back(*i);
					temp = pathc;
					frontiercosts.push_back(temp + 1);
					temppath = finalpath;
					temppath.push_back((*i)->nodeId);
					paths.push_back(temppath);
				}
				else {

					f_queue.insert(f_queue.begin() + index_pri, f_dist);
					frontier.insert(frontier.begin() + index_pri, *i);
					temp = pathc;
					frontiercosts.insert(frontiercosts.begin() + index_pri, temp + 1);
					temppath = finalpath;
					temppath.push_back((*i)->nodeId);
					paths.insert(paths.begin() + index_pri, temppath);


				}

			}
		}
	}
	vector<int> ::iterator j;

	for (j = finalpath.begin()+1; j != finalpath.end();++j)
		mazeText[*j / mazeWidth][*j%mazeWidth] = '.';
	print_maze();
}
	

void Search::multi_search()
{
	nodes_expanded = 0;
	std::vector<adjListNode *> frontier;
	std::vector<int> frontiercosts;
	std::vector<vector<int>> paths;
	std::vector<int> f_queue;
	std::map<int, int> parents;
	std::vector<int> dots_func;
	std::vector<bool*> visit_maze;
	bool * visited = new bool[this->mazeWidth * this->mazeHeight];
	int h_distance;
	int dist_from_start;
	//bool visited[this->mazeWidth * this->mazeHeight];
	std::memset(visited, false, this->mazeWidth * this->mazeHeight);

	visit_maze.push_back(visited);
	int st_linear_idx = this->startPosition.first*this->mazeWidth + this->startPosition.second;
	frontier.push_back(graphVertices[st_linear_idx]);
	frontiercosts.push_back(0);
	int numdots = dotIds.size();
	dots_func = dotIds;
	visited[st_linear_idx] = true;
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	int finald = dotx * mazeWidth + doty;
	int nodes_expanded = 0;
	int current_vertex;
	int pathc = 0;
	int f_dist;
	pair<int, int> st_point(startPosition.first, startPosition.second);
	pair<int, int> f_point(dotx, doty);
	h_distance = mahattan_distance(st_point, f_point);
	dist_from_start = 0;
	f_queue.push_back(h_distance);
	vector<int> finalpath;
	vector<int> numnodes;
	////////HUNGARIAN ALGORITHM
	/*
	vector<vector<int>>dists;
	vector<vector<int>>backupdists;

	vector<int> prevdot;
	int dotdist;
	for (auto j = dotIds.begin(); j != dotIds.end(); j++) 
	{
		vector<int> onedotdist;

		//cout << *j << " ";
		pair<int, int> x_point((*j) / mazeWidth, (*j) / mazeHeight);
		for (auto k = dotIds.begin(); k != dotIds.end();k++)
		{
			pair<int, int> y_point((*k) / mazeWidth, (*k) / mazeHeight);
			dotdist = mahattan_distance(x_point, y_point);
			cout << dotdist << " ";
			if (dotdist == 0)
				dotdist = 1000;
			onedotdist.push_back(dotdist);
		}
		cout << "\n";
		prevdot = onedotdist;
		dists.push_back(onedotdist);
	}
	backupdists = dists;
	int tryagain = 1;
	do {
		for (int i = 0;i < numdots;i++)
		{
			int row_min = 1000;
			for (int j = 0;j < numdots;j++)
			{
				if (dists[i][j] < row_min)
					row_min = dists[i][j];

			}
			//cout << row_min;
			for (int j = 0;j < numdots;j++)
			{
				dists[i][j]= dists[i][j]-row_min;
			}
		}
		for (int i = 0;i < numdots;i++)
		{
			int col_min = 1000;
			for (int j = 0;j < numdots;j++)
			{
				if (dists[j][i] < col_min)
					col_min = dists[j][i];

			}
			for (int j = 0;j < numdots;j++)
			{
				dists[j][i] = dists[j][i] - col_min;
			}
		}
		int numofzeros = 0;
		int coverlines = 0;
		int fl = 0;
		tryagain = 1;
		for (int i = 0;i < numdots;i++)
		{
			for (int j = 0;j < numdots;j++)
			{
				//cout << dists[i][j]<< " ";
				if (dists[i][j] == 0)
					numofzeros++;
			}
			//cout << "\n";
		}
		//cout << "\n" << "\n";
		for (int i = 0;i < numdots;i++)
		{

			for (int j = 0;j < numdots;j++)
			{
				vector<int> coltrack;
				vector<int> rowtrack;
				if (dists[i][j] == 0)
				{
					int k;
					int colzs = 0;
					int rowzs = 0;
					for (k = 0;k < numdots;k++)
					{
						if (dists[k][j] == 0)
						{
							colzs++;
							coltrack.push_back(k*numdots + j);
						}
						if (dists[i][k] == 0)
						{
							rowzs++;
							rowtrack.push_back(i*numdots + k);
						}
					}

					if (colzs >= rowzs)
					{
						for (int m = 0;m < coltrack.size();m++)
						{
							dists[coltrack[m] / numdots][coltrack[m] % numdots] = -1;
							numofzeros = numofzeros - colzs;
							coverlines++;
						}
					}
					else
					{
						for (int m = 0;m < rowtrack.size();m++)
						{
							dists[rowtrack[m] / numdots][rowtrack[m] % numdots] = -1;
							numofzeros = numofzeros - rowzs;
							coverlines++;
						}
					}
					
				}
			}
		}
		//cout << coverlines;
		if (coverlines >= numdots)
		{
			tryagain = 0;
		}
	} while (tryagain);

	for (int i = 0;i < numdots;i++)
	{
		for (int j = 0;j < numdots;j++)
		{
			if (dists[i][j] == -1)
			{
				for (int k = 0;k < numdots;k++)
				{
					//if (dists[k][j] == -1 && k != i)
					//	dists[k][j] = 0;
					//if (dists[i][k] == -1 && k != j)
					//	dists[i][k] = 0;
				}
			}
			else {
				backupdists[i][j] = 0;
			}
		}
	}
	for (int i = 0;i < numdots;i++)
	{
		for (int j = 0;j < numdots;j++)
		{
			cout <<backupdists[i][j]<< " ";
		}
		cout << "\n";
	}
	*/
	//////////////
	finalpath.push_back(st_linear_idx);
	paths.push_back(finalpath);
	numnodes.push_back(numdots);
	while (!frontier.empty())
	{

		adjListNode * v = frontier.front();
		pathc = frontiercosts.front();
		finalpath = paths.front();
		numdots= numnodes.front();
		visited = visit_maze.front();
		//NICE ASTAR ANIMATION
		//if (v->nodeId != st_linear_idx)
		//mazeText[v->nodeId / mazeWidth][v->nodeId%mazeWidth] = 'a' -numdots;
		//print_maze();
		//std::this_thread::sleep_for(std::chrono::milliseconds(300));

		current_vertex = v->nodeId;
		//RESET VISITED AFTER FINDING DOT
		for (auto i = dots_func.begin(); i != dots_func.end(); i++) {
			//cout << *i << " ";

			if (current_vertex == (*i))
			{
				dots_func.erase(std::remove(dots_func.begin(), dots_func.end(), current_vertex), dots_func.end());
				std::memset(visited, false, this->mazeWidth * this->mazeHeight);
				--numdots;
				break;
			}
		}
		//cout << "\n";

		if (numdots == 0)
		{
			cout << "ASTAR_search" << endl;
			cout << "Path cost: " << pathc - 1 << endl;
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			break;
		}

		frontier.erase(frontier.begin());
		frontiercosts.erase(frontiercosts.begin());
		paths.erase(paths.begin());
		f_queue.erase(f_queue.begin());
		numnodes.erase(numnodes.begin());
		visit_maze.erase(visit_maze.begin());
		vector<adjListNode *> ::iterator i;
		for (i = v->neighbours.begin(); i != v->neighbours.end(); ++i)
		{
			int temp;
			vector<int> temppath;
			if (!visited[(*i)->nodeId])
			{
				//cout << (*i)->nodeId / mazeWidth << " " << (*i)->nodeId % mazeWidth<<":";
				nodes_expanded++;
				visited[(*i)->nodeId] = true;
				int index_pri = 0;
				pair<int, int> i_point((*i)->nodeId / mazeWidth, (*i)->nodeId %mazeWidth);
				h_distance = dist_dots((*i)->nodeId, dots_func);
				dist_from_start = mahattan_distance(st_point, i_point);
				f_dist = h_distance + dist_from_start;

				auto ir = f_queue.begin();
				//F_QUEUE HOLDS THE F DISTANCES AND SORTS THEM
				if (!f_queue.empty())
				{
					if (f_dist < *ir)
					{
						index_pri = 0;

					}
					else {
						//FIND WHERE TO INSERT
						for (ir = f_queue.begin(); ir != f_queue.end(); ++ir)
							if (f_dist < *ir)
							{
								break;
							}
						index_pri = ir - f_queue.begin();
					}
				}
				//IF FOR PUSH AT END(INDEX ERRORS) OR PUSH IN MIDDLE
				if (f_queue.empty() || index_pri == f_queue.size())
				{
					//cout << "here";
					f_queue.push_back(f_dist);
					frontier.push_back(*i);
					temp = pathc;
					frontiercosts.push_back(temp + 1);
					numnodes.push_back(numdots);
					visit_maze.push_back(visited);
					temppath = finalpath;
					temppath.push_back((*i)->nodeId);
					paths.push_back(temppath);
				}
				else {

					f_queue.insert(f_queue.begin() + index_pri, f_dist);
					frontier.insert(frontier.begin() + index_pri, *i);
					temp = pathc;
					numnodes.insert(numnodes.begin() + index_pri, numdots);
					visit_maze.insert(visit_maze.begin() + index_pri, visited);
					frontiercosts.insert(frontiercosts.begin() + index_pri, temp + 1);
					temppath = finalpath;
					temppath.push_back((*i)->nodeId);
					paths.insert(paths.begin() + index_pri, temppath);


				}
				//vector<adjListNode *> ::iterator j;
				//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				/*for (j = frontier.begin() + 1; j != frontier.end();++j)
					cout << (*j)->nodeId/mazeWidth<<" "<<(*j)->nodeId%mazeWidth <<" ";
				cout << "\n";
				*/

			}
		}		//	cout << "\n";

		//cout << pathc<< " "<< numdots<< "\n";
		/*vector<int> ::iterator j;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		for (j = finalpath.begin() + 1; j != finalpath.end();++j)
			mazeText[*j / mazeWidth][*j%mazeWidth] = 's';
		print_maze();
		reset_graph();*/
	}
	vector<int> ::iterator j;

	for (j = finalpath.begin() + 1; j != finalpath.end();++j)
		mazeText[*j / mazeWidth][*j%mazeWidth] = 's';
	print_maze();
}

int Search::dist_dots(int agentId, std::vector<int> dots)
{
	//FIND DIST FROM POINT TO ALL OTHER POINTS?
	//restart visited after each point
	int totdis = 0;
	int dotdist = 0;
	pair<int, int> agent_point(agentId/mazeWidth, agentId%mazeWidth);
	int total = 0;
	for (auto i = dots.begin(); i != dots.end(); i++) {
		pair<int, int> f_point((*i)/mazeWidth, (*i)/mazeHeight);
		dotdist = mahattan_distance(agent_point, f_point);
		totdis += dotdist;

	}

	return totdis;
	/*int *myInts= new int[dotIds.size()];
	for (auto i = dotIds.begin(); i != dotIds.end(); i++) {
		cout << (*i) <<"\n";
		//myInts[j] = *i;
		j++;
	}
	/*do {
		for (int k = 0;k < dotIds.size();k++)
			cout << myInts[k] << " ";
		cout << "\n";
	} while (std::next_permutation(myInts, myInts + dotIds.size()));*/
}


void Search::print_maze()
{
	for (int i = 0; i < mazeWidth*mazeHeight; i++)
	{
		if (i%mazeWidth == 0)
			std::cout << std::endl;
		std::cout << mazeText[i / mazeWidth][i%mazeWidth];
	}
	std::cout << std::endl;
}

void Search::reset_graph()
{
	for (int i = 0; i < mazeWidth*mazeHeight; i++)
	{
		if (mazeText[i / mazeWidth][i%mazeWidth] == '.')
		{
			mazeText[i / mazeWidth][i%mazeWidth] = ' ';
		}
	}
	int dotx = this->dotPositions[0].first;
	int doty = this->dotPositions[0].second;
	mazeText[dotx][doty] = '.';
}

float Search::mahattan_distance(pair<int, int> i_point, pair<int, int> f_point)
{
	return std::abs((i_point.first - f_point.first)) + std::abs((i_point.second - f_point.second));
}

float Search::start_distance(pair<int, int> i_point, pair<int, int> f_point)
{
	return 0;
}

void test_pq(std::priority_queue<adjListNode, std::vector<adjListNode>, std::greater<adjListNode> > open)
{
	adjListNode toInsert[10];
	for (int i = 0; i < 10; i++)
	{
		toInsert[i].h_distance = i + 10;
		open.push(toInsert[i]);
	}

	for (int j = 0; j < 10; j++)
	{
		adjListNode top = open.top();
		cout << top.h_distance << endl;
		open.pop();
	}
}
