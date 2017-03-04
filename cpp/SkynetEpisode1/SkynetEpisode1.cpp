#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;


struct Link
{
	Link(int _destination)
	{ 
		destination = _destination;
		usable = true;
		weight = 1;
	}

	void Break() { usable = false; }

	int destination;
	int weight;
	bool usable;
	
};

struct Node
{
	Node()
	{
		links.clear();
	}


	vector<Link> links;
};

void BreakLink(vector<Node> &originalNodes, int node1, int node2)
{
	for (size_t i = 0; i < originalNodes[node1].links.size(); ++i)
	{
		Link *l = &originalNodes[node1].links[i];
		if (l->destination == node2)
		{
			l->Break();
			break;
		}
	}

	for (size_t i = 0; i < originalNodes[node2].links.size(); ++i)
	{
		Link *l = &originalNodes[node2].links[i];
		if (l->destination == node1)
		{
			l->Break();
			break;
		}
	}
}


void dijkstra(vector<Node> &originalNodes, stack<int> &outPath, int sourceNode, vector<int> &outPredecessors)
{
	vector<bool> visited(originalNodes.size());
	vector<int> distance(originalNodes.size());
	vector<int> predecessor(originalNodes.size());

	for (size_t i = 0; i < originalNodes.size(); ++i)
	{
		distance[i] = 1000000;
		visited[i] = false;
		predecessor[i] = -1;
	}

	vector<int> nodes;

	visited[sourceNode] = true;
	distance[sourceNode] = 0;
	predecessor[sourceNode] = 0;
	nodes.push_back(sourceNode);

	while (nodes.empty() == false)
	{
		int currentNode = nodes.front();
		nodes.erase(nodes.begin());

		for (size_t i = 0; i < originalNodes[currentNode].links.size(); ++i)
		{
			Link *l = &originalNodes[currentNode].links[i];
			if (l->usable == true)
			{
				const int destination = l->destination;
				if (visited[destination] == false 
					|| distance[currentNode] + l->weight < distance[destination])
				{
					nodes.push_back(destination);
					visited[destination] = true;
					predecessor[destination] = currentNode;
					distance[destination] = distance[currentNode] + l->weight;
				}
			}
		}

		std::sort(nodes.begin(), nodes.end());
	}

	outPredecessors = predecessor;
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();

	vector<Node> nodeGraph(N);
	vector<int> exits(E);

    for (int i = 0; i < L; i++) {
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2; cin.ignore();

		nodeGraph[N1].links.push_back(Link(N2));
		nodeGraph[N2].links.push_back(Link(N1));
    }

    for (int i = 0; i < E; i++) {
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
		exits[i] = EI;
    }

    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();

		int breakNode1;
		int breakNode2;
		size_t shortestPath = 10000000;

		vector<int> shortestPreds(nodeGraph.size());
		stack<int> path;
		dijkstra(nodeGraph, path, SI, shortestPreds);

		for (size_t i = 0; i < exits.size(); ++i)
		{
			stack<int> path;
			int predIndex = exits[i];
			path.push(exits[i]);
			bool validPath = true;
			while (predIndex != SI)
			{
				int p = shortestPreds[predIndex];
				if (p >= 0)
				{
					predIndex = p;
					path.push(predIndex);
				}
				else
				{
					validPath = false;
					break;
				}
				
			}
			
			if (validPath == true && path.size() < (size_t)shortestPath)
			{
				shortestPath = path.size();

				int sourceNode = path.top();
				path.pop();
				int closestNode = path.top();
				path.pop();
				breakNode1 = sourceNode;
				breakNode2 = closestNode;
			}
		}

		BreakLink(nodeGraph, breakNode1, breakNode2);
		

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << breakNode1 << " " << breakNode2 << endl; // Example: 0 1 are the indices of the nodes you wish to sever the link between
    }
}