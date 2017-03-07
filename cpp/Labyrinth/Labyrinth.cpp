#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int R; // number of rows.
	int C; // number of columns.
	int A; // number of rounds between the time the alarm countdown is activated and the time the alarm goes off.
	cin >> R >> C >> A; cin.ignore();

	// game loop
	while (1) {
		int KR; // row where Kirk is located.
		int KC; // column where Kirk is located.
		cin >> KR >> KC; cin.ignore();
		for (int i = 0; i < R; i++) {
			string ROW; // C of the characters in '#.TC?' (i.e. one line of the ASCII maze).
			cin >> ROW; cin.ignore();
		}

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;

		cout << "RIGHT" << endl; // Kirk's next move (UP DOWN LEFT or RIGHT).
	}
}