#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

#define TEST_INPUT 0
#if TEST_INPUT == 1

static const string INPUT_WORDS[] =
{
	/*[0]*/ "satire",
	/*[1]*/ "sitar",
	/*[2]*/ "ruse",
	/*[3]*/ "ira",
	/*[4]*/ "ar",
};

static const string INPUT_LETTERS = "aretsui";

#endif

struct WordData
{
	WordData(int idx, int s)
		: index(idx)
		, score(s)
	{

	}

	WordData()
	{
		score = 0;
		index = 3000000;
	}

	int score;
	int index;
};


std::map<char, int> scoreMap;

void BuildScoreMap()
{
	scoreMap['e'] = 1;
	scoreMap['a'] = 1;
	scoreMap['i'] = 1;
	scoreMap['o'] = 1;
	scoreMap['n'] = 1;
	scoreMap['r'] = 1;
	scoreMap['t'] = 1;
	scoreMap['l'] = 1;
	scoreMap['s'] = 1;
	scoreMap['u'] = 1;
	scoreMap['d'] = 2;
	scoreMap['g'] = 2;
	scoreMap['b'] = 3;
	scoreMap['c'] = 3;
	scoreMap['m'] = 3;
	scoreMap['p'] = 3;
	scoreMap['f'] = 4;
	scoreMap['h'] = 4;
	scoreMap['v'] = 4;
	scoreMap['w'] = 4;
	scoreMap['y'] = 4;
	scoreMap['k'] = 5;
	scoreMap['j'] = 8;
	scoreMap['x'] = 8;
	scoreMap['q'] = 10;
	scoreMap['z'] = 10;
}

int GetScore(const string &word)
{
	int score = 0;
	for (int i = 0; i < word.size(); i++)
	{
		score += scoreMap[word[i]];
	}

	return score;
}

struct Comparator
{
	static bool sortScore(const string &lhs, const string &rhs)
	{
		return GetScore(rhs) < GetScore(lhs);
	}
};



#define MAX_LETTER_COUNT		7

class Dictionnary
{
public:
	Dictionnary()
	{
		for (int i = 0; i < MAX_LETTER_COUNT; i++)
		{
			bestScoresPerLetterCount[i] = 0;
		}	
	}

	void AddWord(const string W, int index)
	{
		const int score = GetScore(W);
		scores[W] = WordData(index, score);

		bestScoresPerLetterCount[W.size() - 1] = std::max(bestScoresPerLetterCount[W.size() - 1], score);
		words[W.size() - 1].push_back(W);

		//cerr << W << "=" << score << endl;
	}

	int BestScore(int letterCount)
	{
		return bestScoresPerLetterCount[letterCount - 1];
	}

	void Optimize()
	{
		for (int i = 0; i < MAX_LETTER_COUNT; i++)
		{
			std::sort(words[i].begin(), words[i].end(), Comparator::sortScore);
		}
	}

	int GetBestWord(int letterCount, const string letters, string *bestWord)
	{
		int bestScore = -1;
		for (int i = 0; i < words[letterCount - 1].size(); i++)
		{
			const string w = words[letterCount - 1][i];
			if (CanMake(letters, w) == true && scores[w].score > bestScore)
			{
				*bestWord = w;
				bestScore = scores[w].score;
				//break;
			}
		}

		return bestScore;
	}

	void GetWordData(string w, int *outIndex, int *outScore)
	{
		*outIndex = scores[w].index;
		*outScore = scores[w].score;
	}

protected:

	bool CanMake(string letters, const string word)
	{
		bool allGood = true;

		for (int i = 0; i < word.size() && allGood == true; i++)
		{
			char l = word[i];

			size_t ind = letters.find_first_of(l, 0);
			if (ind != string::npos)
			{
				letters.erase(ind, 1);
			}
			else
			{
				allGood = false;
				break;
			}
		}

		return allGood;
	}

public:

protected:

	int bestScoresPerLetterCount[MAX_LETTER_COUNT];
	vector<string> words[MAX_LETTER_COUNT];
	std::map<string, WordData> scores;

};


/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	BuildScoreMap();
	Dictionnary dictionnary;
	
	int N;
	cin >> N; cin.ignore();
	std::cerr << "Cnt:" << N << endl;
	
	for (int i = 0; i < N; i++) 
	{
		string W;
#if TEST_INPUT == 1
		W = INPUT_WORDS[i];
#else
		getline(cin, W);
#endif

		if (W.size() <= MAX_LETTER_COUNT)
		{
			dictionnary.AddWord(W, i);

			std::cerr << "/*[" << i << "]*/ \"" << W << "\"," << endl;
		}
	}

	//dictionnary.Optimize();

	string LETTERS;
#if TEST_INPUT == 1
	LETTERS = INPUT_LETTERS;
#else
	getline(cin, LETTERS);
#endif

	cerr << LETTERS << endl;

	string bestWord;
	int bestScore = 0;
	vector<string> bestWords;
	for (int i = MAX_LETTER_COUNT; i >= 1; i--)
	{
		if (dictionnary.BestScore(i) >= bestScore)
		{
			int score = dictionnary.GetBestWord(i, LETTERS, &bestWord);
			if (score >= 0)
			{
				bestScore = std::max(bestScore, score);
				bestWords.push_back(bestWord);
			}
		}
	}

	//Find first word in dictionnary
	int bestIndex = 300000;
	int bs = 0;
	string firstBestWord;
	for (int i = 0; i < bestWords.size(); i++)
	{
		string w = bestWords[i];
	
		
		
		int index;
		int sc;
		dictionnary.GetWordData(w, &index, &sc);

		cerr << "#" << i << ":" << w << " SC:" << sc << " Idx:" << index << endl;

		if (sc == bs)
		{
			if (index < bestIndex)
			{
				bestIndex = index;
				firstBestWord = w;
			}
		}
		else if (sc > bs)
		{
			bs = sc;
			bestIndex = index;
			firstBestWord = w;
		}
	}

	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	cerr << "Best:" << bestWord << endl;
	cout << firstBestWord << endl;
}