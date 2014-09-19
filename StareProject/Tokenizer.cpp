#include "Tokenizer.h"
#include <sstream>


Tokenizer::Tokenizer(string Document)
{
	nextChar = 0;
}


Tokenizer::~Tokenizer()
{
}

vector<string> Tokenizer::getNextSentence()
{
	vector<string> words;

	return words;
}

vector<string> Tokenizer::getCommands(string cmdStr)
{
	vector<string> words;
	stringstream ss;

	// I was too lazy to implement a full state machine.  However...
	// Uses 3 states to determine the action at a character position.  They are:
	// inWord==false, inQuotes==false :  this state means that any non Alphanums are dumped until a proper alphanum is encountered
	// inWord==true, inQuotes==false  :  This state meant that any alphanums are added to the current word until a non-alphanum is encountered
	// inWord==true, inQuotes==false  :  Anything, whether alphanum or not, is added to the current word until another '"' is encountered.

	bool inWord = false;   // if inWord, all alphanumeric 
	bool inQuotes = false; // if inQuotes is true, all characters are recorded into the current ss until another quote is encountered.

	for (unsigned int i = 0; i < cmdStr.size(); i++)
	{
		if (!inWord)
		{
			if (isAlphaNumeric(cmdStr[i]))
			{
				// start word
				ss << cmdStr[i];
				inWord = true;
			}
			else if (cmdStr[i] == '"')
			{
				// start quotes
				inWord = true;
				inQuotes = true;
			}
		}
		else
		{
			// inWord is true
			if (inQuotes)
			{
				if (cmdStr[i] != '"')
					ss << cmdStr[i];
				else
				{
					// the quotes have ended
					words.push_back(ss.str());
					ss.str("");  // empty ss
					inWord = false;
					inQuotes = false;

				}
			}
			else
			{
				if (isAlphaNumeric(cmdStr[i]))
					ss << cmdStr[i];
				else
				{
					// the word has ended
					words.push_back(ss.str());
					ss.str("");  // empty ss
					inWord = false;
				}

			}
		}
	}
	
	// The last word can end with the end of the string.  
	// It would not be detected then, so add it here.
	if (inWord)
		words.push_back(ss.str());

	return words;
}

bool Tokenizer::isAlphaNumeric(char c)
{
	if (((c >= 'a') && (c <= 'z')) ||
		((c >= 'A') && (c <= 'Z')) ||
		((c >= '0') && (c <= '9')))
		return true;
	else
		return false;
}