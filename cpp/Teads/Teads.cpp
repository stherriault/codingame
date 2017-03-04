#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <ctime>

#define USE_GRAPHVIZ_DEBUG		0
#define TEST_INPUT				0


#if USE_GRAPHVIZ_DEBUG == 1
#include <gvc.h>
#endif

using namespace std;




struct Node;

struct Node
{
	Node(int _id)
	{
		id = _id;
		connections.clear();
		Reset();
	}

	void Reset()
	{
		visited = false;
		dead = false;
	}

	int id;
	bool visited;
	bool dead;
	vector<Node *> connections;
};

int FindBestNode(vector<Node*> &dataset)
{
	int retval = 0;

	//Mark leaf as dead
	vector<Node*>::iterator iter = dataset.begin();
	for (iter = dataset.begin(); iter != dataset.end() && dataset.size() > 1; iter++)
	{
		Node *n = *iter;
		if (n->connections.size() <= 1)
		{
			n->dead = true;
		}
	}

	//Clear out dead leafs and where they are linked
	for (iter = dataset.begin(); iter != dataset.end() && dataset.size() > 1; )
	{
		Node *n = *iter;
		if (n->dead == true)
		{
			iter = dataset.erase(iter);
		}
		else
		{
			vector<Node*>::iterator subiter;
			for (subiter = n->connections.begin(); subiter != n->connections.end(); )
			{
				if ((*subiter)->dead == true)
				{
					subiter = n->connections.erase(subiter);
				}
				else
				{
					subiter++;
				}
			}

			iter++;
		}


	}

	if (dataset.size() > 1)
	{
		retval = FindBestNode(dataset);
	}

	return dataset[0]->id;
}

int RecursiveComputeSearchDepth(Node * sourceNode, int actualDepth, int actualBestDepth, bool *outEarlyExit)
{
	int depth = 0;

	sourceNode->visited = true;

	//Loop through all nodes associated with current node, if not visited and usable
	for (size_t i = 0; i < sourceNode->connections.size() && *outEarlyExit == false; ++i)
	{
		Node *destination = sourceNode->connections[i];

		//If this not hasn't been visited, add it to the search tree
		if (destination->visited == false)
		{
			int newDepth = actualDepth + 1;
			if (destination->connections.size() > 1)
			{
				newDepth = RecursiveComputeSearchDepth(destination, actualDepth + 1, actualBestDepth, outEarlyExit);
			}

			depth = max(depth, newDepth);

			//If this case depth is already greater or equal, no need to continue
			if (depth >= actualBestDepth)
			{
				*outEarlyExit = true;
				break;
			}
		}
	}

	return depth;
}

struct Link
{
	Link(int n1, int n2)
	{
		node1 = n1;
		node2 = n2;
	}

	int node1;
	int node2;
};

#if TEST_INPUT == 1

static const Link s_Links[] =
{
	/*[0]*/{ 6,8 },
	/*[1]*/{ 15,1 },
	/*[2]*/{ 1,14 },
	/*[3]*/{ 0,3 },
	/*[4]*/{ 15,13 },
	/*[5]*/{ 9,15 },
	/*[6]*/{ 2,5 },
	/*[7]*/{ 14,10 },
	/*[8]*/{ 4,9 },
	/*[9]*/{ 7,2 },
	/*[10]*/{ 8,7 },
	/*[11]*/{ 3,4 },
	/*[12]*/{ 1,6 },
};


#endif


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
	int start_s = clock();
    int n; // the number of adjacency relations

#if TEST_INPUT == 1
	n = (sizeof(s_Links) / sizeof(Link));

#if USE_GRAPHVIZ_DEBUG == 1
	Agraph_t *gvzGraph;
	vector<Agnode_t *> gvzNodes(n+1);
	vector<Agedge_t *> gvzEdges(n);

	/* set up a graphviz context - but only once even for multiple graphs */
	static GVC_t *gvc;

	if (!gvc)
		gvc = gvContext();

	gvzGraph = agopen("g", Agundirected, 0);
#endif

#else
    cin >> n; cin.ignore();
#endif

	vector<Node*> nodeGraph;
	vector<Node*> playground;
	cerr << "Link count " << n << endl;

    for (int i = 0; i < n; i++) 
	{
        int xi; // the ID of a person which is adjacent to yi
        int yi; // the ID of a person which is adjacent to xi

#if TEST_INPUT == 1
		xi = s_Links[i].node1;
		yi = s_Links[i].node2;

#if USE_GRAPHVIZ_DEBUG == 1
		string node1 = std::to_string(s_Links[i].node1);
		string node2 = std::to_string(s_Links[i].node2);

		gvzNodes[s_Links[i].node1] = agnode(gvzGraph, const_cast<char*>(node1.c_str()), 1);
		gvzNodes[s_Links[i].node2] = agnode(gvzGraph, const_cast<char*>(node2.c_str()), 1);
		gvzEdges[i] = agedge(gvzGraph, gvzNodes[s_Links[i].node1], gvzNodes[s_Links[i].node2], 0, 1);
#endif

#else
		cin >> xi >> yi; cin.ignore();
#endif
        

		std::cerr << "/*[" << i << "]*/ {" << xi << "," << yi << "}," << endl;

		while (nodeGraph.size() < (xi + 1))
		{
			Node *n = new Node(nodeGraph.size());
			nodeGraph.push_back(n);

			Node *n2 = new Node(playground.size());
			playground.push_back(n2);
		}
		while (nodeGraph.size() < (yi + 1))
		{
			Node *n = new Node(nodeGraph.size());
			nodeGraph.push_back(n);
			
			Node *n2 = new Node(playground.size());
			playground.push_back(n2);
		}

		nodeGraph[xi]->connections.push_back(nodeGraph[yi]);
		nodeGraph[yi]->connections.push_back(nodeGraph[xi]);

		playground[xi]->connections.push_back(playground[yi]);
		playground[yi]->connections.push_back(playground[xi]);
    }

#if TEST_INPUT == 1

#if USE_GRAPHVIZ_DEBUG == 1
	/* Compute a layout using layout engine from command line args */
	gvLayoutJobs(gvc, gvzGraph);
	/* Write the graph according to -T and -o options */
	gvRenderJobs(gvc, gvzGraph);
	/* Free layout data */
	gvFreeLayout(gvc, gvzGraph);
#endif

#endif

	int bestNode = FindBestNode(playground);

	bool earlyExit = false;
	int minimumTime = 150000000;
	minimumTime = RecursiveComputeSearchDepth(nodeGraph[bestNode], 0, minimumTime, &earlyExit);

    // The minimal amount of steps required to completely propagate the advertisement
    cout << minimumTime << endl;

#if USE_GRAPHVIZ_DEBUG == 1
	agclose(gvzGraph);
#endif
}
