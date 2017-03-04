#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


#define TEST_INPUT	0

#if TEST_INPUT == 1

static const string map[] =
{
	"###############",
	"#  #@#I  T$#  #",
	"#  #    IB #  #",
	"#  #     W #  #",
	"#  #      ##  #",
	"#  #B XBN# #  #",
	"#  ##      #  #",
	"#  #       #  #",
	"#  #     W #  #",
	"#  #      ##  #",
	"#  #B XBN# #  #",
	"#  ##      #  #",
	"#  #       #  #",
	"#  #     W #  #",
	"#  #      ##  #",
	"#  #B XBN# #  #",
	"#  ##      #  #",
	"#  #       #  #",
	"#  #       #  #",
	"#  #      ##  #",
	"#  #  XBIT #  #",
	"#  #########  #",
	"#             #",
	"# ##### ##### #",
	"# #     #     #",
	"# #     #  ## #",
	"# #     #   # #",
	"# ##### ##### #",
	"#             #",
	"###############",
};


#endif


struct Point
{
	Point()
	{
		x = 0;
		y = 0;
	}

	Point(int _x, int _y)
		: x(_x)
		, y(_y)
	{
		
	}

	int x;
	int y;
};

namespace eDirection
{
	enum Type
	{
		INVALID,
		NORTH,
		WEST,
		SOUTH, 
		EAST,
	};
}

struct sMove
{
	Point p;
	eDirection::Type dir;
};

class Game
{
public:

	Game()
	{

	}

	void Initialize(int lines, int columns)
	{
		cerr << "L:" << lines << " C:" << columns << endl;
		m_Map.resize(lines);
		for (int i = 0; i < lines; i++)
		{
			m_Map[i].resize(columns);
		}

		teleporterOne = Point(-1, -1);
		teleporterTwo = Point(-1, -1);

	}

	void SetMapItem(int line, int column, char item)
	{
		m_Map[line][column] = item;

		if (item == Game::TELEPORTER)
		{
			if (teleporterOne.x == -1)
			{
				teleporterOne.x = line;
				teleporterOne.y = column;
			}
			else
			{
				teleporterTwo.x = line;
				teleporterTwo.y = column;
			}
		}
	}

	Point TeleporterOne() const { return teleporterOne; }
	Point TeleporterTwo() const { return teleporterTwo; }

	char GetMapItem(int line, int column)
	{
		return m_Map[line][column];
	}

	bool CanMoveToPoint(const Point &pos)
	{
		const char item = GetMapItem(pos.x, pos.y);
		switch (item)
		{
		case Game::WALL:
		case Game::OBSTACLE:
		{
			return false;
		}

		default:
		{
			return true;
		}
		}
	}

	bool IsObstacle(const Point &pos)
	{
		const char item = GetMapItem(pos.x, pos.y);
		if (item == Game::OBSTACLE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool BreakObstacle(const Point &pos)
	{
		const char item = GetMapItem(pos.x, pos.y);
		if (item == Game::OBSTACLE)
		{
			SetMapItem(pos.x, pos.y, Game::NOTHING);
			return true;
		}
		else
		{
			return false;
		}
	}



protected:

public:
	static const char BENDER = '@';
	static const char NORTH_ITEM = 'N';
	static const char WEST_ITEM = 'W';
	static const char SOUTH_ITEM = 'S';
	static const char EAST_ITEM = 'E';
	static const char SUICIDE_BOOTH = '$';
	static const char WALL = '#';
	static const char OBSTACLE = 'X';
	static const char NOTHING = ' ';
	static const char TELEPORTER = 'T';
	static const char BEER = 'B';
	static const char INVERTER = 'I';

protected:

	vector<vector<char>> m_Map;

	Point teleporterOne;
	Point teleporterTwo;
};

class Bender
{
public:

	Bender(Game *g)
	{
		m_eDirection = eDirection::SOUTH;
		Point p(0, 0);
		SetActualPos(p);
		isDead = false;

		breakerMode = false;
		inverterMode = false;

		pGame = g;
	}

	void SetActualPos(Point p)
	{
		m_PreviousPos = p;
		m_ActualPos = p;
	}

	Point ActualPos() { return m_ActualPos; }

	void DrinkBeer()
	{
		breakerMode = !breakerMode;
	}

	void TriggerInverterMode()
	{
		inverterMode = !inverterMode;
	}

	void ChangeDirection(eDirection::Type newDir)
	{
		m_eDirection = newDir;
	}

	bool HasBreakerMode() const { return breakerMode; }
	bool IsDead() const { return isDead; }

	void Kill()
	{
		isDead = true;
	}

	void Move()
	{
		eDirection::Type nextDir = eDirection::INVALID;

		if (IsDead() == false)
		{
			bool moveDone = false;
			while (moveDone == false)
			{
				Point nextPos = ComputeNextPosition();
				if (pGame->CanMoveToPoint(nextPos) == true)
				{
					ProcessMovement(nextPos);
					moveDone = true;
				}
				else
				{
					if (pGame->IsObstacle(nextPos) == true)
					{
						if (HasBreakerMode() == true)
						{
							pGame->BreakObstacle(nextPos);
							ProcessMovement(nextPos);
							moveDone = true;
						}
					}
				}

				if (moveDone == false)
				{
					SwitchDirection(nextDir);
					nextDir = m_eDirection;
				}
			}
			
		}
	}

	void ProcessMovement(Point &newPos)
	{
		SetActualPos(newPos);
		sMove m;
		m.p = newPos;
		m.dir = m_eDirection;
		m_Moves.push_back(m);
	}

	int MoveCount()
	{
		return m_Moves.size();
	}

	sMove GetMove(int index)
	{
		return m_Moves[index];
	}

protected:

	Point ComputeNextPosition()
	{
		switch (m_eDirection) 
		{
		case eDirection::EAST:
		{
			return Point(m_ActualPos.x + 1, m_ActualPos.y);
			break;
		}
		case eDirection::SOUTH:
		{
			return Point(m_ActualPos.x, m_ActualPos.y + 1);
			break;
		}
		case eDirection::NORTH:
		{
			return Point(m_ActualPos.x, m_ActualPos.y - 1);
			break;
		}
		case eDirection::WEST:
		{
			return Point(m_ActualPos.x - 1, m_ActualPos.y);
			break;
		}

		default:
		{
			return Point(m_ActualPos.x, m_ActualPos.y);
		}
		}
	}

	void SwitchDirection(eDirection::Type dir)
	{
		if (inverterMode == false)
		{
			switch (dir)
			{
			case eDirection::INVALID:
			{
				ChangeDirection(eDirection::SOUTH);
				break;
			}
			case eDirection::EAST:
			{
				ChangeDirection(eDirection::NORTH);
				break;
			}
			case eDirection::SOUTH:
			{
				ChangeDirection(eDirection::EAST);
				break;
			}
			case eDirection::NORTH:
			{
				ChangeDirection(eDirection::WEST);
				break;
			}
			case eDirection::WEST:
			{
				ChangeDirection(eDirection::SOUTH);
				break;
			}
			}
		}
		else
		{
			switch (dir)
			{
			case eDirection::INVALID:
			{
				ChangeDirection(eDirection::WEST);
				break;
			}
			case eDirection::EAST:
			{
				ChangeDirection(eDirection::SOUTH);
				break;
			}
			case eDirection::SOUTH:
			{
				ChangeDirection(eDirection::WEST);
				break;
			}
			case eDirection::NORTH:
			{
				ChangeDirection(eDirection::EAST);
				break;
			}
			case eDirection::WEST:
			{
				ChangeDirection(eDirection::NORTH);
				break;
			}
			}
		}
	}
	

public:

protected:
	Point m_PreviousPos;
	Point m_ActualPos;
	eDirection::Type m_eDirection;
	Game *pGame;

	bool breakerMode;
	bool inverterMode;
	bool isDead;

	vector<Point> m_Visited;
	vector<sMove> m_Moves;
};

void ProcessBenderPosition(Game &g, Bender &b)
{
	const char item = g.GetMapItem(b.ActualPos().x, b.ActualPos().y);
	if (item == Game::BEER)
	{
		b.DrinkBeer();
	}
	else if (item == Game::EAST_ITEM)
	{
		b.ChangeDirection(eDirection::EAST);
	}
	else if (item == Game::NORTH_ITEM)
	{
		b.ChangeDirection(eDirection::NORTH);
	}
	else if (item == Game::WEST_ITEM)
	{
		b.ChangeDirection(eDirection::WEST);
	}
	else if (item == Game::SOUTH_ITEM)
	{
		b.ChangeDirection(eDirection::SOUTH);
	}
	else if (item == Game::SUICIDE_BOOTH)
	{
		b.Kill();
	}
	else if (item == Game::INVERTER)
	{
		b.TriggerInverterMode();
	}
	else if (item == Game::TELEPORTER)
	{
		const Point teleOne = g.TeleporterOne();
		const Point teleTwo = g.TeleporterTwo();
		const Point benderPos = b.ActualPos();
		if (benderPos.x == teleOne.x && benderPos.y == teleOne.y)
		{
			b.SetActualPos(teleTwo);
		}
		else
		{
			b.SetActualPos(teleOne);
		}
	}
	
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
	Game game;
	Bender bender(&game);

    int L;
    int C;

#if TEST_INPUT == 0
    cin >> L >> C; cin.ignore();
#else
	L = sizeof(map) / sizeof(map[0]);
	C = map[0].size();
#endif

	game.Initialize(C, L);

    for (int i = 0; i < L; i++) {
        string row;

#if TEST_INPUT == 0
        getline(cin, row);
#else
		row = map[i];
#endif

		cerr << "\"" << row << "\"," << endl;

		for (int j = 0; j < C; j++)
		{
			game.SetMapItem(j, i, row[j]);

			if (row[j] == Game::BENDER)
			{
				bender.SetActualPos(Point(j, i));
			}
		}
    }

	bool exit = false;
	while (exit == false)
	{
		bender.Move();
		ProcessBenderPosition(game, bender);

		if (bender.IsDead() == true)
		{
			cerr << "Moves:" << bender.MoveCount() << endl;
			exit = true;
		}
		else if (bender.MoveCount() > 200)
		{
			exit = true;
		}
	}

	if (bender.IsDead() == true)
	{
		for (int i = 0; i < bender.MoveCount(); i++)
		{
			string direction;
			sMove m = bender.GetMove(i);
			switch (m.dir)
			{
			case eDirection::EAST:
			{
				direction = "EAST";
				break;
			}
			case eDirection::SOUTH:
			{
				direction = "SOUTH";
				break;
			}
			case eDirection::NORTH:
			{
				direction = "NORTH";
				break;
			}
			case eDirection::WEST:
			{
				direction = "WEST";
				break;
			}
			}

			cout << direction << endl;
		}
	}
	else
	{
		cout << "LOOP" << endl;
	}
}