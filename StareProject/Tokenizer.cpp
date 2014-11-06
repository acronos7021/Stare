#include "Tokenizer.h"
#include "Database.h"
#include <sstream>
using namespace std;

//takes care of everything aside from and empty lines in the document ATM

Tokenizer::Tokenizer()// string Document)
{
//	filename = Document;
	proceed = false;
	punctuation[0] = '.';
	punctuation[1] = '!';
	punctuation[2] = '?';
};

Tokenizer:: ~Tokenizer()
{
	//nada
}

bool Tokenizer::checkIgnore(char c)
{
	if ((c >= 33 && c <= 47 || c >= 58 && c <= 64 || c >= 91 && c <= 96 || c >= 123 && c <= 126 || c >= 128 || c >= 10 && c <= 13) && (c != 39 && c != 96))
	{
		return true;
	}
	return false;
}

bool Tokenizer::checkPunctuation(char c)
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

// Preassigned token types
// 1 = Carrage Return
// 2 = tab

std::vector <std::vector<int>> Tokenizer::tokenizeDoc(string Document)
{
	//StyleDatabase db = StyleDatabase::getInstance();

	sentences.clear();
	ifstream fname(Document);
	string currline;
	//vector <string> curr;
	vector <int> curr;
	vector <string> temp;
	if (!fname.is_open())
	{
		printf("\nAn error has occured and the file does not exist!!!\n");
		return sentences;
	}
	StyleDatabase db;
	db.open();

	while (!fname.eof())
	{
		if (fname.peek() == '\n' || fname.peek() == '\r')
		{
			curr.push_back(1); // use /n instead of \n for illustration puposes only
		}
		fname >> currline;
		curr.push_back(db.GetToken(currline));
		for (unsigned i = 0; i < currline.size(); i++)
		{
			//if (currline[i] == 39 || currline[i] == 239 || currline[i] == 96)
			//{
			//	curr.pop_back();
			//	currline[i] = 'Z';
			//	curr.push_back(currline);
			//}

			if (checkIgnore((char)currline[i]) || checkPunctuation((char)currline[i]))
			{
				curr.pop_back();
				temp.clear();
				temp = getWord(currline);
				for (unsigned int i = 0; i < temp.size(); i++)
				{
					curr.push_back(db.GetToken(temp[i]));
				}
				if (checkPunctuation((char)currline[i]))
				{
					proceed = true;
				}
				break;
			}
		}
		if (proceed)
		{
			sentences.push_back(curr);
			curr.clear();
			proceed = false;
		}
	}
	db.FlushTokenCache();
	db.close();
	return sentences;
} //end of method

vector<string> Tokenizer::getWord(string word)
{
	vector<string> v;
	vector<char>letters;
	string temp;

	for (unsigned int i = 0; i < word.size(); i++)
	{	
		letters.push_back(word[i]);
	}
	for (unsigned int i = 0; i < letters.size(); i++)
	{
		if (!checkIgnore(letters[i]))
		{
			temp.push_back(letters[i]);
		}
		else
		{
			if (temp != "")
			{
				v.push_back(temp);
			}

			temp.clear();
			temp = letters[i];

			if (temp != "")
			{
				v.push_back(temp);
			}
			temp.clear();
		}
	}
	return v;
}

void Tokenizer::print_BAV()
{
	for (unsigned int i = 0; i < sentences.size(); i++)
	{
		for (unsigned int j = 0; j < sentences[i].size(); j++)
		{
			cout << sentences[i][j] << " ";
		}
	}
}

vector <int> Tokenizer::getNextSentence()
{
	vector<int> temp;
	if (sentences.size() < 1)
	{
		return temp;
	}
	else
	{
		temp = sentences[0];
		sentences.erase(sentences.begin());
		return temp;
	}
}

/*int main()
{
string testfile = "text.txt";
Sentence test(testfile);

test.tokenizeDoc();
test.print_BAV();

cout << endl;
system("pause");
}*/



//#include "Tokenizer.h"
//#include <sstream>
//
//
//Tokenizer::Tokenizer(string Document)
//{
//	nextChar = 0;
//}
//
//
//Tokenizer::~Tokenizer()
//{
//}
//
//vector<string> Tokenizer::getNextSentence()
//{
//	vector<string> words;
//
//	return words;
//}
//
//vector<string> Tokenizer::getCommands(string cmdStr)
//{
//	vector<string> words;
//	stringstream ss;
//
//	// I was too lazy to implement a full state machine.  However...
//	// Uses 3 states to determine the action at a character position.  They are:
//	// inWord==false, inQuotes==false :  this state means that any non Alphanums are dumped until a proper alphanum is encountered
//	// inWord==true, inQuotes==false  :  This state meant that any alphanums are added to the current word until a non-alphanum is encountered
//	// inWord==true, inQuotes==true  :  Anything, whether alphanum or not, is added to the current word until another '"' is encountered.
//
//	bool inWord = false;   // if inWord, all alphanumeric 
//	bool inQuotes = false; // if inQuotes is true, all characters are recorded into the current ss until another quote is encountered.
//
//	for (unsigned int i = 0; i < cmdStr.size(); i++)
//	{
//		if (!inWord)
//		{
//			if (isAlphaNumeric(cmdStr[i]))
//			{
//				// start word
//				ss << cmdStr[i];
//				inWord = true;
//			}
//			else if (cmdStr[i] == '"')
//			{
//				// start quotes
//				inWord = true;
//				inQuotes = true;
//			}
//		}
//		else
//		{
//			// inWord is true
//			if (inQuotes)
//			{
//				if (cmdStr[i] != '"')
//					ss << cmdStr[i];
//				else
//				{
//					// the quotes have ended
//					words.push_back(ss.str());
//					ss.str("");  // empty ss
//					inWord = false;
//					inQuotes = false;
//
//				}
//			}
//			else
//			{
//				if (isAlphaNumeric(cmdStr[i]))
//					ss << cmdStr[i];
//				else
//				{
//					// the word has ended
//					words.push_back(ss.str());
//					ss.str("");  // empty ss
//					inWord = false;
//				}
//
//			}
//		}
//	}
//	
//	// The last word can end with the end of the string.  
//	// It would not be detected then, so add it here.
//	if (inWord)
//		words.push_back(ss.str());
//
//	return words;
//}
//
//bool Tokenizer::isAlphaNumeric(char c)
//{
//	if (((c >= 'a') && (c <= 'z')) ||
//		((c >= 'A') && (c <= 'Z')) ||
//		((c >= '0') && (c <= '9')))
//		return true;
//	else
//		return false;
//}