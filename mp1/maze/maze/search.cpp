#include "search.h"
#include "Disjoint.h"
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
				temppath.push_back((v)->nodeId);
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
		int last = 1;
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
				int last = 1;
				auto ir = f_queue.begin();
				//F_QUEUE HOLDS THE F DISTANCES AND SORTS THEM
				if (!f_queue.empty())
				{
					if (f_dist < *ir)
					{
						last = 0;
						index_pri = 0;

					}
					else {
						//FIND WHERE TO INSERT
						for (ir = f_queue.begin(); ir != f_queue.end(); ++ir)
							if (f_dist < *ir)
							{
								last = 0;
								break;
							}
						index_pri = ir - f_queue.begin();
					}
				}
				//IF FOR PUSH AT END(INDEX ERRORS) OR PUSH IN MIDDLE
				if (f_queue.empty() || last)
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
	std::vector<vector<int>> dotsonpath;

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

	int nodes_expanded = 0;
	int current_vertex;
	int pathc = 0;
	int f_dist;
	pair<int, int> st_point(startPosition.first, startPosition.second);
	dist_from_start = 0;

	vector<int> finalpath;
	vector<int> numnodes;

	////////PRIM"S MST

	vector<vector<int>>dists;

	vector <pair<int, pair<int,int>>> MSTedges; //weight, src, dst

	int dotdist;
	for (auto j = dotIds.begin(); j != dotIds.end(); j++)
	{
		vector<int> onedotdist;

		//cout << *j << " ";
		pair<int, int> x_point((*j) / mazeWidth, (*j) % mazeWidth);
		for (auto k = dotIds.begin(); k != dotIds.end();k++)
		{
			pair<int, int> y_point((*k) / mazeWidth, (*k) % mazeWidth);
			dotdist = mahattan_distance(x_point, y_point);
			//cout << dotdist << " ";
			onedotdist.push_back(dotdist);
			if(dotdist!=0)
				MSTedges.push_back({ dotdist,{ *j, *k } });
		}
		//cout << "\n";
		dists.push_back(onedotdist);
	}
	int other_dot;
	/*for (int m = 0;m < numdots;m++)
	{
		other_dot = dotIds[m];
		pair<int, int> dotpt(other_dot/mazeWidth, other_dot%mazeWidth);
		dotdist = mahattan_distance(st_point, dotpt);
		MSTedges.push_back({ dotdist,{st_linear_idx, other_dot} });

	}*/
	sort(MSTedges.begin(), MSTedges.end());
	vector <pair<int, pair<int, int>>> MSTsubg; //weight, src, dst
	vector< pair<int, pair<int,int>> >::iterator it;
	vector<int> dots_reached;
	for (it = MSTedges.begin(); it != MSTedges.end(); it++)
	{
		int u = it->second.first;
		int v = it->second.second;
		int travel;
		int cyc = 0;
		int src_f = 0;
		int dest_f = 0;
		pair<int, pair<int, int>> edge = *it;

		vector< pair<int, pair<int, int>> >::iterator it2;
		for (it2 = MSTsubg.begin(); it2 != MSTsubg.end(); it2++)
		{

			if (u == it2->second.first || u== it2->second.second)
			{
				src_f = 1;
			}
			if (v == it2->second.second || v== it2->second.first )
			{
				dest_f = 1;
			}
			if (src_f && dest_f)
				cyc = 1;

		}
		if (!cyc)
		{
			MSTsubg.push_back(edge);
		}

	}
	for (it = MSTsubg.begin(); it != MSTsubg.end(); it++)
	{
		int u = it->second.first;
		int v = it->second.second;
		cout << it->first << " " << u << " " << v;
		cout << '\n';
	}
	/*
	sort the edges of G in increasing order of cost
	keep a subgraph S of G, initially empty
	for each edge e in sorted order
	if the endpoints of e are disconnected in S(i.e., does not form a cycle)
	add e to S
	return S	*/
	///////////////////////////////
	vector<int> visitedDots;
	std::map<int, int> dotMap;
	for (int i = 0; i < dots_func.size(); i++) {
		dotMap[dots_func[i]] = i;
	}
	int mstWeight = kruskalMST(MSTedges, dots_func.size(), dotMap);
	h_distance = dist_dots(st_linear_idx, st_point, dots_func, MSTsubg, mstWeight);



	f_queue.push_back(h_distance);
	finalpath.push_back(st_linear_idx);
	paths.push_back(finalpath);
	numnodes.push_back(numdots);
	dotsonpath.push_back(dots_func);

	while (!frontier.empty())
	{

		adjListNode * v = frontier.front();
		pathc = frontiercosts.front();
		finalpath = paths.front();
		numdots= numnodes.front();
		visited = visit_maze.front();
		dots_func = dotsonpath.front();
		//NICE ASTAR ANIMATION
		//if (v->nodeId != st_linear_idx)
		//mazeText[v->nodeId / mazeWidth][v->nodeId%mazeWidth] = 'a' -numdots;
		//print_maze();
		//std::this_thread::sleep_for(std::chrono::milliseconds(300));

		current_vertex = v->nodeId;
		


		dotsonpath.erase(dotsonpath.begin());
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
				vector<int> rchdots;
				rchdots = dotIds;

				for (auto z = dots_func.begin(); z != dots_func.end(); z++) {
					//cout << *i << " ";

					if ((*i)->nodeId == (*z))
					{
						dots_func.erase(std::remove(dots_func.begin(), dots_func.end(), (*i)->nodeId), dots_func.end());
						std::memset(visited, false, this->mazeWidth * this->mazeHeight);
						visited[(*i)->nodeId] = true;
						--numdots;
						for (int q = 0;q < dots_func.size();q++)
						{

							rchdots.erase(std::remove(rchdots.begin(), rchdots.end(), dots_func[q]), rchdots.end());

						}
						for (int q = 0;q < rchdots.size();q++)
						{
							//cout<<rchdots[q]<<" ";
							visited[rchdots[q]] = true;
						}
						//cout << "\n";
						for (int k = 0; k < MSTedges.size();) {
							if (MSTedges[k].second.first == current_vertex || MSTedges[k].second.second == current_vertex) {
								MSTedges.erase(MSTedges.begin() + k);
							}
							else {
								k++;
							}
						}

						std::map<int, int> dotMapUpdate;
						for (int i = 0; i < dots_func.size(); i++) {
							dotMapUpdate[dots_func[i]] = i;
						}
						mstWeight = kruskalMST(MSTedges, dots_func.size(), dotMapUpdate);
						break;

					}

				}

				visited[(*i)->nodeId] = true;
				

				//cout << (*i)->nodeId / mazeWidth << " " << (*i)->nodeId % mazeWidth<<":";
				nodes_expanded++;
				int index_pri = 0;
				pair<int, int> i_point((*i)->nodeId / mazeWidth, (*i)->nodeId %mazeWidth);
				h_distance = dist_dots((*i)->nodeId, st_point,dots_func, MSTsubg, mstWeight);
				dist_from_start = mahattan_distance(st_point, i_point);
				f_dist = h_distance + dist_from_start;
				int last = 1;
				auto ir = f_queue.begin();
				//F_QUEUE HOLDS THE F DISTANCES AND SORTS THEM
				if (!f_queue.empty())
				{
					if (f_dist < *ir)
					{
						index_pri = 0;
						last = 0;
					}
					else {
						//FIND WHERE TO INSERT
						for (ir = f_queue.begin(); ir != f_queue.end(); ++ir)
							if (f_dist < *ir)
							{
								last = 0;
								break;
							}
						index_pri = ir - f_queue.begin();
					}
				}

				//IF FOR PUSH AT END(INDEX ERRORS) OR PUSH IN MIDDLE
				if (f_queue.empty() || last)
				{
					//cout << "here";
					f_queue.push_back(f_dist);
					frontier.push_back(*i);
					temp = pathc;
					frontiercosts.push_back(temp + 1);
					dotsonpath.push_back(dots_func);
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
					dotsonpath.insert(dotsonpath.begin()+ index_pri,dots_func);
					frontiercosts.insert(frontiercosts.begin() + index_pri, temp + 1);
					temppath = finalpath;
					temppath.push_back((*i)->nodeId);
					paths.insert(paths.begin() + index_pri, temppath);


				}
		
				

			}
		}		
		if (numdots == 0)
		{
			cout << "ASTAR_search" << endl;
			cout << "Path cost: " << pathc - 1 << endl;
			std::cout << "Found the dot" << std::endl;
			cout << "nodes_expanded: " << nodes_expanded << std::endl;
			break;
		}
		
		
		//	cout << "\n";
		vector<adjListNode *> ::iterator j;
		//std::this_thread::sleep_for(std::chrono::milliseconds(400));
		//for (j = frontier.begin() + 1; j != frontier.end();++j)
		//	cout << (*j)->nodeId/ mazeWidth << " " << (*j)->nodeId%mazeWidth << " ";
		//	cout << "\n";
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
		mazeText[*j / mazeWidth][*j%mazeWidth] = '.';
	print_maze();
}

int Search::dist_dots(int agentId, pair<int,int> st_point,std::vector<int> dots, vector <pair<int, pair<int, int>>> MSTsubg, int mstWeight)
{
	int totdis = 0;
	int dotdist = 0;
	pair<int, int> agent_point(agentId / mazeWidth, agentId%mazeWidth);
	int stdist = 0;
	int min = 1000;
	int min2 = 1000;
	// dist to nearest unvisited + estimate to travel all unvisited(MST)
	for (auto i = dots.begin(); i != dots.end(); i++) {
		pair<int, int> f_point((*i) / mazeWidth, (*i) / mazeHeight);
		dotdist = mahattan_distance(agent_point, f_point);
		//stdist = mahattan_distance(st_point, f_point);
		stdist = mahattan_distance(st_point, f_point);
		if (dotdist < min)
			min = dotdist;
		if (stdist < min2)
			min2 = stdist;

	}
	vector< pair<int, pair<int, int>> >::iterator it;
	int vis;
	int fis;
	//dist to all unvisited in MST
	/*
	for (it = MSTsubg.begin(); it != MSTsubg.end(); it++)
	{
	vis = it->second.second;
	fis = it->second.first;
	for (int m = 0;m < dots.size();m++)
	{
	if (dots[m] == vis || dots[m]== fis)
	totdis = totdis + it->first;
	}
	}*/
	return mstWeight + min + min2;
	/*
	//FIND DIST FROM POINT TO ALL OTHER POINTS?
	//restart visited after each point
	int totdis = 0;
	int dotdist = 0;
	pair<int, int> agent_point(agentId/mazeWidth, agentId%mazeWidth);
	int stdist = 0;
	int min = 1000;
	int min2 = 1000;
	// dist to nearest unvisited + estimate to travel all unvisited(MST)
	for (auto i = dots.begin(); i != dots.end(); i++) {
		pair<int, int> f_point((*i)/mazeWidth, (*i)/mazeHeight);
		dotdist = mahattan_distance(agent_point, f_point);
		stdist = mahattan_distance(st_point, f_point);
		if (dotdist < min)
			min = dotdist;
		if (stdist < min2)
			min2 = stdist;

	}
	vector< pair<int, pair<int, int>> >::iterator it;
	int vis;
	int fis;
	//dist to all unvisited in MST
	for (it = MSTsubg.begin(); it != MSTsubg.end(); it++)
	{
		vis = it->second.second;
		fis = it->second.first;
		for (int m = 0;m < dots.size();m++)
		{
			if (dots[m] == vis || dots[m]== fis)
				totdis = totdis + it->first;
		}
	}
	return totdis+min2+min;*/
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
int Search::kruskalMST(vector <pair<int, pair<int, int>>> edges, int numVertices, map<int, int> dotM)
{
	int mst_wt = 0; // Initialize result

					// Sort edges in increasing order on basis of cost
	sort(edges.begin(), edges.end());

	// Create disjoint sets
	DisjointSets ds(numVertices);

	// Iterate through all sorted edges
	vector< pair<int, pair<int, int>> >::iterator it;
	for (it = edges.begin(); it != edges.end(); it++)
	{
		int u = dotM[it->second.first];
		int v = dotM[it->second.second];

		int set_u = ds.DisjointSets::find(u);
		int set_v = ds.DisjointSets::find(v);

		// Check if the selected edge is creating
		// a cycle or not (Cycle is created if u
		// and v belong to same set)
		if (set_u != set_v)
		{
			// Current edge will be in the MST
			// so print it
			cout << u << " - " << v << endl;

			// Update MST weight
			mst_wt += it->first;

			// Merge two sets
			ds.DisjointSets::merge(set_u, set_v);
		}
	}
	cout << endl;
	return mst_wt;
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
