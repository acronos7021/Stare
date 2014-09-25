#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "DataStructs.h"

class Tokenizer
{
public:
	bool proceed;
	std::vector <std::vector<std::string>> sentences;
	char punctuation[3];

	Tokenizer(std::string Document);
	~Tokenizer();

	bool checkIgnore(char c);
	bool checkPunctuation(char c);

	std::vector<std::string> getWord(std::string word);
	std::string checkTabRet(std::string word);

	//tokenizes the entire document and fills up the sentences vector
	void tokenizeDoc();
	void print_BAV();

	//returns a single sentence from the document
	//returns an empty vector if there is no sentence left
	std::vector<std::string> getNextSentence();

	static vector<string> getCommands(string cmdStr);
	static bool isAlphaNumeric(char c);

private:
	std::string filename;
};


//#pragma once
//
//#include "DataStructs.h"
//
//class Tokenizer
//{
//public:
//	Tokenizer(string Document);
//	~Tokenizer();
//
//	// returns a single sentence from the Document
//	// returns an empty vector if there is no next sentence.
//	vector<string> getNextSentence();
//
//	static vector<string> getCommands(string cmdStr);
//	static bool isAlphaNumeric(char c);
//
//private:
//	int nextChar;
//
//};

