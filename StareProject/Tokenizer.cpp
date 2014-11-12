#include "Tokenizer.h"
#include "Stopwatch.h"
#include "DataStructs.h"
using namespace std;

Tokenizer::Tokenizer()
{
	index = 0;
	//punctuation[0] = '.';
	//punctuation[1] = '!';
	//punctuation[2] = '?';
};

Tokenizer:: ~Tokenizer()
{

}

inline bool Tokenizer::checkIgnore(char c)
{
	// Words are only made out of letters, so only letters are considered word tokens.  
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return false;
	else
		return true;
}

inline bool Tokenizer::checkPunctuation(char c)
{

	if (c == '.') return true;
	if (c == '!') return true;
	if (c == '?') return true;
	return false;
}

std::deque<std::vector<int>> Tokenizer::tokenizeFile(string filename)
{
	return tokenizeDoc(readFile(filename));
}

std::deque<std::vector<int>> Tokenizer::tokenizeDoc(string document)
{
	index = 0;  
	sentID.clear();
	loc ind;
	ind.start = &document[0];
	ind.end = &document[0];
	char* done = &document[document.size() ];
	vector <int> currSentence; // vector <int> tmp;

	while (ind.end != done)
	{
		if (checkPunctuation((char)*ind.end))
		{
			// We've reached the end of a sentence
			if (ind.start != ind.end)
				currSentence.push_back(tdb.GetToken(ind.toString())); // push last word
			currSentence.push_back((int) *ind.end); // push the punctuation mark
			sentID.push_back(currSentence); // push the sentence
			currSentence.clear();
			ind.start = ++ind.end;
		}
		else if (checkIgnore((char)*ind.end))
		{
			// We've reached the end of a word
			if (ind.start != ind.end)
				currSentence.push_back(tdb.GetToken(ind.toString())); // push word
			currSentence.push_back((int)*ind.end); // push the symbol character.
			ind.start = ++ind.end;
		}
		else
		{
			// it must be part of a word so get the next char
			++ind.end;
		}
	} 
	// we've reached the end of the document.
	// it is possible the last word didn't end in a sentence, so clean up that last word and sentence
	if (ind.start != ind.end)
		currSentence.push_back(tdb.GetToken(ind.toString()));
	if (currSentence.size() != 0)
		sentID.push_back(currSentence);
	tdb.FlushTokenCache();
	return sentID;
}

string Tokenizer::rebuildSent(vector<int>sent)
{
	stringstream ss;
	for (unsigned int i = 0; i < sent.size(); i++)
		ss << tdb.GetString(sent[i]);
	return ss.str();
}


void Tokenizer::save_BAV(std::string filename)
{
	using std::ofstream;
	ofstream t(filename);
	if (!t.is_open())
	{
		cout << "can't open '" << filename << "' file for output" << endl;
	}
	else
	{
		for (int i = 0; i < (int)sentID.size(); i++)
			t << rebuildSent(sentID[i]);
	}
}//end of rebuildDoc

vector<int> Tokenizer::getNextSentence() //vector<int> Tokenizer::getNextSentence2()
{
	vector<int> temp; // vector<int> temp
	if (index <(int) sentID.size()) //(index > sentID.size() - 1)
	{
		temp = sentID[index]; //temp = sentID[index];
		index++;
		//sentences.erase(sentences.begin());
	}
	return temp;
}


string Tokenizer::readFile(string fileName)
{
	using std::ifstream;
	ifstream t(fileName);
    int myerror = errno;
	stringstream buffer;
	string documentText ="";
	if (!t.is_open())
	{
		cout << "can't open '" << fileName << "' file for input" << endl;
	}
	else
	{
		buffer << t.rdbuf();
		documentText = buffer.str();
	}
	return documentText;
}

