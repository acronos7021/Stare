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

private:
	int nextChar;

};

