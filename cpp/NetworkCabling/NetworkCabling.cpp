#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


struct House
{
	House(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	int64_t x;
	int64_t y;
};

struct Comparator
{
	static bool sortXCoord(House* &lhs, House* &rhs)
	{
		return rhs->x < lhs->x;
	}

	static bool sortYCoord(House* &lhs, House* &rhs)
	{
		return rhs->y < lhs->y;
	}
};

class Game
{
public:
	Game()
	{
		minx = 1000000;
		maxx = 0;
		miny = 1000000;
		maxy = 0;

		totalY = 0;
		totalX = 0;
		averageY = 0;
		averageX = 0;

		bestLength = 0;
		bestYpos = 0;
	}

	static int64_t minx;
	static int64_t maxx;
	static int64_t miny;
	static int64_t maxy;
	static vector<House*> houses;

	static float totalY;
	static float totalX;
	static float averageY;
	static float averageX;

	static int64_t bestLength;
	static int64_t bestYpos;
};

int64_t Game::minx;
int64_t Game::maxx;
int64_t Game::miny;
int64_t Game::maxy;
vector<House*> Game::houses;

int64_t Game::bestLength;
int64_t Game::bestYpos;

float Game::totalY;
float Game::totalX;
float Game::averageY;
float Game::averageX;

void UpdateBestLength(const int64_t length, const int64_t pos)
{
	if (length < Game::bestLength)
	{
		Game::bestLength = length;
		Game::bestYpos = pos;
	}
}

void FindTopBoundary(int64_t &ioMin, int64_t &ioMax, int64_t &ioActualBB, int64_t &ioPreviousLength)
{
	int64_t previousGoodLim = ioMin;
	bool earlyExitDetected = false;

	bool done = false;
	while (done == false)
	{
		int64_t cableLength = Game::maxx - Game::minx;
		for (int i = 0; i < Game::houses.size() && cableLength < Game::bestLength; i++)
		{
			House *h = Game::houses[i];
			cableLength += abs(h->y - ioActualBB);
		}
		
		if (earlyExitDetected == true)
		{
			done = true;
		}
		else if (cableLength < ioPreviousLength) //As long as the length is shorter, we move up
		{
			previousGoodLim = ioMin;
			ioMin = ioActualBB;
			

			//Corner case going up, we want to be able to test for maxy
			if (abs(ioMax - ioMin) > 1)
			{
				ioActualBB = ioMin + abs(ioMax - ioMin) * 0.5;
			}
			else
			{
				ioActualBB = ioMax;
				earlyExitDetected = true;
			}
		}
		else
		{
			//We've gone too far, stamp Y pos as max and put back our last good Min to continue the search
			ioMin = previousGoodLim;
			ioMax = ioActualBB;
			done = true;
		}

		UpdateBestLength(cableLength, cableLength);

		ioPreviousLength = cableLength;
	}
}

void FindBottomBoundary(int64_t &ioMin, int64_t &ioMax, int64_t &ioActualBB, int64_t &ioPreviousLength)
{
	int64_t previousGoodLim = ioMax;
	bool done = false;
	while (done == false)
	{
		int64_t cableLength = Game::maxx - Game::minx;
		for (int i = 0; i < Game::houses.size() && cableLength < Game::bestLength; i++)
		{
			House *h = Game::houses[i];
			cableLength += abs(h->y - ioActualBB);
		}

		//As long as the length is shorter, we move up
		if (cableLength < ioPreviousLength)
		{
			previousGoodLim = ioMax;
			ioMax = ioActualBB;
			ioActualBB = ioMin + abs(ioMax - ioMin) * 0.5;
		}
		else
		{
			//We've gone too far, stamp Y pos as max
			ioMax = previousGoodLim;
			ioMin = ioActualBB;
			done = true;
		}

		UpdateBestLength(cableLength, cableLength);

		ioPreviousLength = cableLength;
	}
}

void FindBestLength()
{
	int state = 1;

	int64_t actualBB = Game::miny;
	int64_t min = Game::miny;
	int64_t max = Game::maxy;
	int64_t actualLength = 100000000000;
	int singleSpaceCounter = 0;
	Game::bestLength = actualLength;

	bool done = false;

	//Verify special conditions
	if (Game::houses.size() <= 1)
	{
		Game::bestLength = 0;
		Game::bestYpos = Game::miny;
		done = true;
	}

	while (done == false)
	{
		//Verify admissible conditions
		if (abs(max - min) <= 1)
		{
			++singleSpaceCounter;
			if (singleSpaceCounter > 1)
			{
				done = true;
			}
		}
		else
		{
			if (state == 1)
			{
				FindTopBoundary(min, max, actualBB, actualLength);
				actualBB = min + abs(max - min) * 0.5;
				state = 0;
			}
			else
			{
				FindBottomBoundary(min, max, actualBB, actualLength);
				actualBB = min + abs(max - min) * 0.5;
				state = 1;
			}
		}
	}
}

//#define TEST_INPUT
#ifdef TEST_INPUT

static const House s_Houses[] = 
{
	/*[0]*/{ 1,2 },
	/*[1]*/{ 0,0 },
	/*[2]*/{ 2,2 },
};

static int divider = 1;

#endif

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
	Game g;
	int N;

#ifdef TEST_INPUT
	N = sizeof(s_Houses) / sizeof(House);
#else
	cin >> N; cin.ignore();
#endif

	for (int i = 0; i < N; i++) 
	{
#ifdef TEST_INPUT
		int64_t X = s_Houses[i].x / divider;
		int64_t Y = s_Houses[i].y / divider;
#else
		int64_t X;
		int64_t Y;
		cin >> X >> Y; cin.ignore();
#endif
		
		Game::houses.push_back(new House(X, Y));

		Game::minx = min(Game::minx, X);
		Game::maxx = max(Game::maxx, X);
		Game::miny = min(Game::miny, Y);
		Game::maxy = max(Game::maxy, Y);

		Game::totalX += X;
		Game::totalY += Y;

		std::cerr << "/*[" << i << "]*/ {" << X << "," << Y << "}," << endl;
	}

    
    

	Game::averageX = Game::totalX / (float)N;
	Game::averageY = Game::totalY / (float)N;

	
	FindBestLength();

	cerr << "AvgY=" << Game::averageY << endl;
	cerr << "Y=" << Game::bestYpos << endl;


    cout << Game::bestLength << endl;
}