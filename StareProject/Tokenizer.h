#pragma once

#include "DataStructs.h"

class Tokenizer
{
public:
	Tokenizer(string Document);
	~Tokenizer();

	// returns a single sentence from the Document
	// returns an empty vector if there is no next sentence.
	vector<string> getNextSentence();

	static vector<string> getCommands(string cmdStr);
	static bool isAlphaNumeric(char c);

private:
	int nextChar;

};

