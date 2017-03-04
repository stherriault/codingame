#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

#define TEST_INPUT		0
#if TEST_INPUT == 1

static const int S_HEIGHT = 4;
static const int S_WIDTH = 4;
static const string S_NUMERALS[] =
{
	".oo.o...oo..ooo.oooo....o...oo..ooo.oooo....o...oo..ooo.oooo....o...oo..ooo.oooo",
	"o..o................____________________________________________________________",
	".oo.....................................________________________________________",
	"............................................................____________________",
};

static int FIRST_NUM_LINES = 12;
static const string S1_NUMERALS[] =
{
	"oooo",
	"....",
	"....",
	"....",
	"ooo.",
	"____",
	"....",
	"....",
	".oo.",
	"o..o",
	".oo.",
	"....",
};

static int SEC_NUM_LINES = 4;
static const string S2_NUMERALS[] =
{
	"....",
	"____",
	"____",
	"....",
};

static const char OPERATOR = '/';

#endif

static int bad = 0;

class MayanNumber
{
public:
	MayanNumber(int w, int h)
	{
		height = h;
		width = w;
		symbol.resize(height);
		for (int i = 0; i < height; i++)
		{
			symbol[i].resize(w);
		}

		key = 0; 
		value = -1;
	}

	void SetValue(int v)
	{
		value = v;

		if (s_MayanValueMap.find(key) == s_MayanValueMap.end())
		{
			s_MayanValueMap[key] = value;
		}
		else
		{
			bad++;
		}
	}

	void Initialize(const vector<char> &s)
	{
		int index = 0;
		for (int k = 0; k < height; k++)
		{
			for (int j = 0; j < width; j++)
			{
				char c = s[k*width + j];
				symbol[k][j] = c;
				key += c + (c * k) ^ j;
				//key += c;
			}
		}
	}

	int GetKey() const { return key; }

	void GetSymbol(vector<char> &outSymbol)
	{
		for (int k = 0; k < height; k++)
		{
			for (int j = 0; j < width; j++)
			{
				outSymbol.push_back(symbol[k][j]);
			}
		}
	}

protected:

public:

	static std::map<int, int> s_MayanValueMap;

protected:

	int value;
	int key;

	int height;
	int width;
	vector<vector<char>> symbol;
};

vector<MayanNumber*> g_Numbers;
std::map<int, int> MayanNumber::s_MayanValueMap;


void BuildNumberList(int height, int width, const vector<string> & lines)
{
	for (int i = 0; i < 20; i++)
	{
		MayanNumber *n = new MayanNumber(width, height);
		vector<char> chars;
		for (int k = 0; k < height; k++)
		{
			for (int j = 0; j < width; j++)
			{
				chars.push_back(lines[k][i*width + j]);
			}
		}
		
		n->Initialize(chars);
		n->SetValue(i);
		g_Numbers.push_back(n);
	}
}

int GetMayanNumber(int height, int width, const vector<string> & lines)
{
	int result = 0;
	int numberCount = lines.size() / height;

	vector<int> numbers;
	for (int i = 0; i < numberCount; ++i)
	{
		MayanNumber n(width, height);
		vector<char> chars;
		for (int k = 0; k < height; k++)
		{
			for (int j = 0; j < width; j++)
			{
				chars.push_back(lines[i*height + k][j]);
			}
		}

		n.Initialize(chars);
		numbers.push_back(MayanNumber::s_MayanValueMap[n.GetKey()]);
	}

	int pow = 0;
	for (int i = 0; i < numberCount; i++)
	{
		int ndx = numberCount - 1 - i;
		int n = numbers[ndx];
		result += n * (int)std::pow(20, i);
	}

	return result;
}

int64_t DoOperation(int64_t num1, int64_t num2, char op)
{
	switch (op)
	{
	case '+':
	{
		return num1 + num2;
		break;
	}
	case '-':
	{
		return num1 - num2;
		break;
	}
	case '/':
	{
		return num1 / num2;
		break;
	}
	case '*':
	{
		return num1 * num2;
		break;
	}
	default:
		return num1 + num2;
	}
}

void GetMayanDigits(int64_t number, vector<int> &outDigits)
{
	//Get last pow
	int biggestPow = 0;
	for (int i = 1; i < 63; i++)
	{
		if (number >= (int64_t)pow(20, i) && number < (int64_t)pow(20, i+1))
		{
			biggestPow = i;
			break;
		}
	}

	int loopCount = biggestPow + 1;
	for (int i = 0; i < loopCount; ++i)
	{
		int64_t power = (int64_t)pow(20, biggestPow);
		int64_t count = number / power;
		outDigits.push_back(count);
		number -= count * power;
		biggestPow--;
	}
}

int main()
{
    int L;
    int H;
	vector<string> mayanLines;
    
#if TEST_INPUT == 1
	L = S_WIDTH;
	H = S_HEIGHT;
#else
	cin >> L >> H; cin.ignore();
	cerr << L << " " << H << endl;
#endif
	
	
    for (int i = 0; i < H; i++) 
	{
        string numeral;

#if TEST_INPUT == 1
		numeral = S_NUMERALS[i];
#else
        cin >> numeral; cin.ignore();
		cerr << "\"" << numeral << "\"," << endl;
#endif
		mayanLines.push_back(numeral);
    }

	BuildNumberList(H, L, mayanLines);

    int S1;
	vector<string> num1Lines;

#if TEST_INPUT == 1
	S1 = FIRST_NUM_LINES;
#else
	cin >> S1; cin.ignore();
	cerr << S1 << endl;
#endif

   
    for (int i = 0; i < S1; i++) {
        string num1Line;

#if TEST_INPUT == 1
		num1Line = S1_NUMERALS[i];
#else
		cin >> num1Line; cin.ignore();
		cerr << "\"" << num1Line << "\"," << endl;
#endif
       
		num1Lines.push_back(num1Line);
    }

    int S2;
	vector<string> num2Lines;

#if TEST_INPUT == 1
	S2 = SEC_NUM_LINES;
#else
	cin >> S2; cin.ignore();
	cerr << S2 << endl;
#endif
    
    for (int i = 0; i < S2; i++) {
        string num2Line;

#if TEST_INPUT == 1
		num2Line = S2_NUMERALS[i];
#else
		cin >> num2Line; cin.ignore();
		cerr << "\"" << num2Line << "\"," << endl;
#endif
		num2Lines.push_back(num2Line);
    }

    string operation;
    
#if TEST_INPUT == 1
	operation = OPERATOR;
#else
	cin >> operation; cin.ignore();
	cerr << operation << endl;
#endif
	
	int64_t num1 = GetMayanNumber(H, L, num1Lines);
	int64_t num2 = GetMayanNumber(H, L, num2Lines);
	int64_t result = DoOperation(num1, num2, operation[0]);

	vector<int> mayanDigit;
	GetMayanDigits(result, mayanDigit);

	for (int i = 0; i < mayanDigit.size(); i++)
	{
		MayanNumber *m = g_Numbers[mayanDigit[i]];
		vector<char> c;
		m->GetSymbol(c);
		for (int k = 0; k < H; k++)
		{
			string line;
			for (int j = 0; j < L; j++)
			{
				line += c[k*L + j];
			}

			cout << line << endl;
		}
	}
}