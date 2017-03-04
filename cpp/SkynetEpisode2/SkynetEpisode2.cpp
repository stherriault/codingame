#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

struct sInputLink
{
	sInputLink(const int n1, const int n2)
		: node1(n1)
		, node2(n2)
	{}
	int node1;
	int node2;
};

#define TEST_INPUT 0

#if TEST_INPUT == 1
static const int N = 8;
static const int L = 13;
static const int E = 2;
static const sInputLink INPUT_LINKS[] =
{
	/*[0] = */{ 6, 2 },
	/*[1] = */{ 7, 3 },
	/*[2] = */{ 6, 3 },
	/*[3] = */{ 5, 3 },
	/*[4] = */{ 3, 4 },
	/*[5] = */{ 7, 1 },
	/*[6] = */{ 2, 0 },
	/*[7] = */{ 0, 1 },
	/*[8] = */{ 0, 3 },
	/*[9] = */{ 1, 3 },
	/*[10] = */{ 2, 3 },
	/*[11] = */{ 7, 4 },
	/*[12] = */{ 6, 5 },
};
static const int INPUT_GATEWAYS[] =
{
	/*[0] = */ 4,
	/*[1] = */ 5,
};
#endif

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
#if TEST_INPUT != 1
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();


	cerr << "static const int N = " << N << ";" << endl;
	cerr << "static const int L = " << L << ";" << endl;
	cerr << "static const int E = " << E << ";" << endl;
#endif

	vector<Node> nodeGraph(N);
	vector<int> exits(E);

#if TEST_INPUT != 1
	cerr << "static const sInputLink INPUT_LINKS[] =" << endl;
	cerr << "{" << endl;
#endif

    for (int i = 0; i < L; i++) {

#if TEST_INPUT != 1
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2; cin.ignore();
		cerr << "/*[" << i << "] = */ {" << N1 << ", " << N2 << "}," << endl;
#else
		int N1 = INPUT_LINKS[i].node1;
		int N2 = INPUT_LINKS[i].node2;
#endif

		nodeGraph[N1].links.push_back(Link(N2));
		nodeGraph[N2].links.push_back(Link(N1));
    }

#if TEST_INPUT != 1
	cerr << "};" << endl;


	cerr << "static const int INPUT_GATEWAYS[] =" << endl;
	cerr << "{" << endl;
#endif
    for (int i = 0; i < E; i++) {
#if TEST_INPUT != 1
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
		cerr << "/*[" << i << "] = */ " << EI << "," << endl;
#else
		int EI = INPUT_GATEWAYS[i];
#endif

		exits[i] = EI;
    }
#if TEST_INPUT != 1
	cerr << "};" << endl;
#endif

    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();
		cerr << "SI = " << SI << endl;

		int breakNode1;
		int breakNode2;
		size_t shortestPath = 10000000;

		vector<int> shortestPreds(nodeGraph.size());
		stack<int> path;
		dijkstra(nodeGraph, path, SI, shortestPreds);

		int biggestGatewayConnected = 0;

		//Loop through all exits and check the shortest path between each exit and skynet
		for (size_t i = 0; i < exits.size(); ++i)
		{
			vector<int> path;
			int predIndex = exits[i];
			path.push_back(exits[i]);  //start by pushing the exit
			bool validPath = true;
			while (predIndex != SI)
			{
				int p = shortestPreds[predIndex];
				if (p >= 0)
				{
					predIndex = p;
					path.push_back(predIndex);
				}
				else
				{
					validPath = false;
					break;
				}
				
			}
			

			if (validPath == true)
			{
				if (path.size() == 2)
				{
					cerr << "Early exit" << endl;
					shortestPath = path.size();

					int sourceNode = path[0];
					int closestNode = path[1];
					breakNode1 = sourceNode;
					breakNode2 = closestNode;
					break;
				}
				else
				{
					//count gateway connected
					int gatewayLink = 0;
					int closestNode = path[1];
					for (int i = 0; i < nodeGraph[closestNode].links.size(); ++i)
					{
						Link *l = &nodeGraph[closestNode].links[i];
						if (l->usable == true)
						{
							const int d = l->destination;
							if (std::find(exits.begin(), exits.end(), d) != exits.end())
							{
								gatewayLink++;
							}
						}
					}

					if(gatewayLink > biggestGatewayConnected && path.size() <= shortestPath)
					{
						biggestGatewayConnected = gatewayLink;
						shortestPath = path.size();
						int sourceNode = path[0];
						int closestNode = path[1];
						breakNode1 = sourceNode;
						breakNode2 = closestNode;
					}
				}
				/*if (path.size() < (size_t)shortestPath)
				{
					shortestPath = path.size();

					int sourceNode = path[0];
					int closestNode = path[1];
					breakNode1 = sourceNode;
					breakNode2 = closestNode;
				}
				else if (path.size() < (size_t)(shortestPath + 1))
				{
					shortestPath = path.size();

					int sourceNode = path[0];
					int closestNode = path[1];

					//Count number of link to gateways
					int gatewayLink = 0;
					for (int i = 0; i < nodeGraph[closestNode].links.size(); ++i)
					{
						Link *l = &nodeGraph[closestNode].links[i];
						if (l->usable == true)
						{
							const int d = l->destination;
							if (std::find(exits.begin(), exits.end(), d) != exits.end())
							{
								gatewayLink++;
							}
						}
					}

					if (gatewayLink > 1)
					{
						breakNode1 = sourceNode;
						breakNode2 = closestNode;
						break;
					}
				}*/
			}
		}

		//Verify if one of the node is a gateway
		if (std::find(exits.begin(), exits.end(), breakNode1) != exits.end()
			|| std::find(exits.begin(), exits.end(), breakNode2) != exits.end())
		{
			BreakLink(nodeGraph, breakNode1, breakNode2);

			cout << breakNode1 << " " << breakNode2 << endl; // Example: 0 1 are the indices of the nodes you wish to sever the link between
		}

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        
    }
}