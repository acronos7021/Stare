#include "Tokenizer.h"
#include "Stopwatch.h"
#include "Database.h"
#include "DataStructs.h"
using namespace std;

Tokenizer::Tokenizer()
{
	index = 0;
	punctuation[0] = '.';
	punctuation[1] = '!';
	punctuation[2] = '?';
};

Tokenizer:: ~Tokenizer()
{

}

inline bool Tokenizer::checkIgnore(char c)
{
	if ((c == 32 && c >= 34 && c <= 45 || c == 47 || c >= 58 && c <= 62 || c == 64 || c >= 91 && c <= 96 || c >= 123 && c <= 126 || c >= 128) || (c == '\"' || c == '\,' ||
		c == '\(' || c == '\)' || c == '\'' || c == '\-'))
	{
		return true;
	}
	return false;
}

inline bool Tokenizer::checkPunctuation(char c)
{
	for (unsigned i = 0; i < sizeof(punctuation); i++)
	{
		if (c == (char)punctuation[i])
		{
			return true;
		}
	}
	return false;
}

void Tokenizer::print_BAV()
{
	for (int i = 0; i < (int)sentID.size(); i++)
	{
		for (int j = 0; j < (int)sentID[i].size(); j++)
		{
			cout << sentID[i][j] << " ";
		}
		cout << endl;
	}
}

vector<int> Tokenizer::getNextSentence() //vector<int> Tokenizer::getNextSentence2()
{
	vector<int> temp; // vector<int> temp
	if (index > sentID.size() - 1) //(index > sentID.size() - 1)
	{
		return temp;
	}
	else
	{
		temp = sentID[index]; //temp = sentID[index];
		index++;
		//sentences.erase(sentences.begin());
		return temp;
	}
}

void Tokenizer::tokenizeFileH()
{
	loc* ind = new loc();
	ind->start = &file[0];
	ind->end = &file[0];
	char* done = &file[file.size() - 1];
	vector <int> tmp; // vector <int> tmp;

	while (ind->end != done)
	{
		if (!(ind->start == ind->end && *ind->end == 32))
		{
			if (checkIgnore((char)*ind->end))
			{
				tmp.push_back(db.GetToken(ind->toString()));
				if (*ind->end != 32)
				{
					tmp.push_back(db.GetToken(ind->toStringE()));
				}
				ind->start = ++ind->end;
			}
			else if (checkPunctuation((char)*ind->end))
			{
				tmp.push_back(db.GetToken(ind->toString()));
				tmp.push_back(db.GetToken(ind->toStringE()));
				sentID.push_back(tmp); // sentID.push_back(tmp);
				tmp.clear();
				ind->start = ++ind->end;
			}
			else
			{
				++ind->end;
			}
		}
		else
		{
			ind->start = ++ind->end;
		}
		if (ind->end == done)
		{
			tmp.push_back(db.GetToken(ind->toString()));
			tmp.push_back(db.GetToken(ind->toStringE()));
			sentID.push_back(tmp); // sentID.push_back(tmp);
			tmp.clear();
		}
	} //end of loop
} //end of method

void Tokenizer::tokenizeFile(string filename)
{
	index = 0;
	readFile(filename);
	file.c_str();
	tokenizeFileH();
}

std::deque<std::vector<int>> Tokenizer::tokenizeDoc(string document)
{
	index = 0;
	file = document;
	file.c_str();
	loc* ind = new loc();
	ind->start = &file[0];
	ind->end = &file[0];
	char* done = &file[file.size() - 1];
	vector <int> tmp; // vector <int> tmp;

	while (ind->end != done)
	{
		if (!(ind->start == ind->end && *ind->end == 32))
		{
			if (checkIgnore((char)*ind->end))
			{
				tmp.push_back(db.GetToken(ind->toString()));
				if (*ind->end != 32)
				{
					tmp.push_back(db.GetToken(ind->toStringE()));
				}
				ind->start = ++ind->end;
			}
			else if (checkPunctuation((char)*ind->end))
			{
				tmp.push_back(db.GetToken(ind->toString()));
				tmp.push_back(db.GetToken(ind->toStringE()));
				sentID.push_back(tmp); // sentID.push_back(tmp);
				tmp.clear();
				ind->start = ++ind->end;
			}
			else
			{
				++ind->end;
			}
		}
		else
		{
			ind->start = ++ind->end;
		}
		if (ind->end == done)
		{
			tmp.push_back(db.GetToken(ind->toString()));
			tmp.push_back(db.GetToken(ind->toStringE()));
			sentID.push_back(tmp); // sentID.push_back(tmp);
			tmp.clear();
		}
	} //end of loop
	return sentID;
}

string Tokenizer::rebuildSent(vector<int>sent)
{
	string s;
	char c;
	for (int i = 0; i < sent.size(); i++)
	{
		if (sent[i] < 128 && sent[i] >= 0)
		{
			c = (char)sent[i];
			if (checkPunctuation(c) || checkIgnore(c))
			{
				s.pop_back();
				s += (char)sent[i];
			}
			else
			{
				s += (char)sent[i];
				s += " ";
			}
		}
		else
		{
			s += std::to_string(sent[i]) + " "; //s += db.getString(sent[i]) + " ";
		}
	}
	return s;
}//end of rebuildDoc

void Tokenizer::readFile(string fileName)
{
	using std::ifstream;
	ifstream t(fileName);
	stringstream buffer;
	if (!t.is_open())
	{
		cout << "can't open '" << fileName << "' file for input" << endl;
	}
	else
	{
		buffer << t.rdbuf();
		file = buffer.str();
	}
}

//int main()
//{
//	string testfile = "The Man of Adamant.txt";
//	string testfile2 = "Test.txt";
//	Tokenizer test;
//	Stopwatch s;
//	s.start();
//	test.tokenizeFile(testfile);
//	s.end();
//
//	test.print_BAV();
//	std::cout << "\n\n" << s.getTimeInMicroseconds() << " Ms\n\n";
//	/*
//	if ((c == 32 && c >= 34 && c <= 45 || c == 47 || c >= 58 && c <= 62 || c == 64 || c >= 91 && c <= 96 || c >= 123 && c <= 126 || c >= 128) || (c == '\"' || c == '\,' ||
//	c == '\(' || c == '\)' || c == '\'' || c == '\-'))*/
//
//	/*vector<int>testID;
//	testID.push_back(256);
//	testID.push_back(345);
//	testID.push_back(756);
//	testID.push_back(876);
//	testID.push_back(44);
//	testID.push_back(65);
//	testID.push_back(256);
//	testID.push_back(33);
//	string sentence = test.rebuildDoc(testID);
//
//	cout << sentence << endl;*/
//
//	system("pause");
//}