#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <deque>
#include <stdexcept>
//#include "DataStructs.h"
#include "Database.h"

class Tokenizer
{
public:
	struct loc
	{
		char* start;
		char* end;
		std::string s;
		loc::loc() : start(0), end(0) {};

		std::string toString()
		{
			s.clear();
			while (start != end) // hits the symbol
			{
				s += *start;
				start++;
			}
			return s;
		};

		std::string toStringE()
		{
			s.clear();
			s += *end;
			return s;
		};
	};

	std::string file;
	int index;

	//std::deque<std::vector<std::string>> sentence;
	std::deque<std::vector<int>>sentID;
	char punctuation[3];

	Tokenizer();
	~Tokenizer();

	//@param: char c
	//@return: true if the character is a symbol, false otherwise
	//@logic: assist in identifing if a char is a symbol or not
	inline bool checkIgnore(char c);

	//@param: char c
	//@return: true if the character is a punctuation mark, false otherwise
	//@logic: assist in identifing if a char is a punctuation mark or not
	inline bool checkPunctuation(char c);

	//@param: a string filename
	//@return: void
	//@logic: calls tokenizeFileH() to tokenize the entire document into a deque<vector<int>> sentID
	void tokenizeFile(std::string filename);

	//@param: void
	//@return: void
	//@logic: tokenizes the entire document into a deque<vector<int>> sentID
	void tokenizeFileH();

	//@param: string document (a single string that is the entire document)
	//@return: void
	//@logic: tokenizes the entire document into a deque<vector<int>> sentID
	std::deque<std::vector<int>> tokenizeDoc(std::string document);

	//@param: vector<int> sent
	//@return: a string that is a reconstructed sentence
	//@logic: it reconstructs a sentence based on a vector of word IDs
	std::string rebuildSent(std::vector<int> sent);

	//@param: void
	//@return: a vector of string / ID from the master list
	//@logic: called after a document has been tokenize; it returns the current sentence you are on based on the index
	std::vector<int> getNextSentence(); //std::vector<int>getNextSentence();

	//@param: string filename
	//@return: void
	//@logic: reads a file and puts it into one string
	void readFile(std::string filename);

	//@param: void
	//@return: void
	//@logic: prints the master list
	void print_BAV();
};