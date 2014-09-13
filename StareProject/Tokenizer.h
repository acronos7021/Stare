#pragma once

#include "DataStructs.h"

class Tokenizer
{
public:
	Tokenizer(string Document);
	~Tokenizer();

	// returns a single sentence
	// returns an empty vector i
	vector<string> getNextSentence();

private:
	int nextChar;

};

