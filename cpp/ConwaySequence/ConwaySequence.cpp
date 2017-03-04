#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int R;
    cin >> R; cin.ignore();
    int L;
    cin >> L; cin.ignore();

	string line = std::to_string(R);

	for (int lineC = 1; lineC < L; lineC++)
	{
		int lastDigit = -1;
		int successiveCount = 0;
		int integer = -1;
		bool first = true;

		string newLine;

		for (int i = 0; i < line.size(); i++)
		{
			string digit;
			size_t pos = line.find_first_of(' ', i);
			if (pos != string::npos)
			{
				digit = line.substr(i, pos-i);
				i = pos;
			}
			else
			{
				digit = line.substr(i);
				i = line.size();
			}
			

			if (digit != " ")
			{
				integer = std::stoi(digit, 0, 10);
				if (integer == lastDigit || first == true)
				{
					successiveCount++;

					if (first == true)
					{
						lastDigit = integer;
						first = false;
					}
				}
				else
				{
					//Found new number, process the previous one
					if (newLine.empty() == false)
					{
						newLine += " ";
					}
					newLine += std::to_string(successiveCount) + " " + std::to_string(lastDigit);

					lastDigit = integer;
					successiveCount = 1;
				}
			}
		}

		//Process last number in the string
		if (newLine.empty() == false)
		{
			newLine += " ";
		}
		newLine += std::to_string(successiveCount) + " " + std::to_string(integer);

		line = newLine;
	}
	
    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << line << endl;
}