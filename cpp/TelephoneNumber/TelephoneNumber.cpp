#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Node
{
public:
	static Node* CreateNode()
	{
		ms_NodeCount++;
		return new Node();
	}

	Node * AddNode(int index)
	{
		Node * n = children[index];
		if (n == NULL)
		{
			children[index] = CreateNode();
			n = children[index];
		}

		return n;
	}

	Node *GetChild(int index)
	{
		return children[index];
	}

	static int NodeCount() { return ms_NodeCount; }
private:
	Node()
	{
		children.resize(10);
		for (int i = 0; i < 10; i++)
		{
			children[i] = NULL;
		}
	}

public:

private:
	static int ms_NodeCount;
	vector<Node*> children;

};


class TelephoneNumbers
{
public:
	TelephoneNumbers()
	{
		numbers.resize(10);
		for (int i = 0; i < 10; i++)
		{
			numbers[i] = NULL;
		}
	}

	void AddNumber(string num)
	{
		vector<int> digits;
		for (int j = 0; j < num.size(); j++)
		{
			int digit = std::stoi(num.substr(j, 1), 0, 10);
			digits.push_back(digit);
		}

		//Process first number
		if (digits.size() > 0)
		{
			//Get the root number and create it if it doesn't exist
			Node *actual = numbers[digits[0]];
			if (actual == NULL)
			{
				actual = Node::CreateNode();
				numbers[digits[0]] = actual;
			}

			//Erase first number from list to start recursive actions
			digits.erase(digits.begin());

			AddDigit(actual, digits);
		}
		
		
	}

protected:

	void AddDigit(Node *parent, vector<int> &digits)
	{
		//If there are digits left to add, go on
		if (digits.empty() == false)
		{
			Node* child = parent->AddNode(digits[0]);
			digits.erase(digits.begin());

			AddDigit(child, digits);
		}
	}

protected:
	vector<Node*> numbers;
};




int Node::ms_NodeCount = 0;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
	TelephoneNumbers nbrs;

    int N;
    cin >> N; cin.ignore();
    for (int i = 0; i < N; i++) 
	{
        string telephone;
        cin >> telephone; cin.ignore();
		nbrs.AddNumber(telephone);
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;


    // The number of elements (referencing a number) stored in the structure.
    cout << Node::NodeCount() << endl;
}