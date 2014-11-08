#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <deque>
#include <stdexcept>
//#include "DataStructs.h"
#include "TokenDatabase.h"

class Tokenizer
{
public:
	struct loc
	{
		char* start;
		char* end;

		loc::loc() : start(0), end(0) {};

		std::string toString()
		{
			return (std::string(start, end - start));
		};

		std::string toStringE()  // return string without the last character,
		{
			return (std::string(start, end - start - 1));
		};
	};

	//std::string file;
	int index;

	//std::deque<std::vector<std::string>> sentence;
	std::deque<std::vector<int>>sentID;
	char punctuation[3];

	Tokenizer();
	~Tokenizer();

	TokenDatabase tdb;

	//@param: char c
	//@return: true if the character is a symbol, false otherwise
	//@logic: assist in identifing if a char is a symbol or not
	static inline bool checkIgnore(char c);

	//@param: char c
	//@return: true if the character is a punctuation mark, false otherwise
	//@logic: assist in identifing if a char is a punctuation mark or not
	inline bool checkPunctuation(char c);

	//@param: a string filename
	//@return: void
	//@logic: calls tokenizeFileH() to tokenize the entire document into a deque<vector<int>> sentID
	std::deque<std::vector<int>> tokenizeFile(std::string filename);

	//@param: void
	//@return: void
	//@logic: tokenizes the entire document into a deque<vector<int>> sentID
	void tokenizeFileH();

	//@param: string document (a single string that is the entire document)
	//@return: void
	//@logic: tokenizes the entire document into a deque<vector<int>> sentID
	//std::deque<std::vector<int>> tokenizeDoc(std::string document);
	std::deque<std::vector<int>> tokenizeDoc(std::string &document);

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
	//void readFile(std::string filename);
	std::string readFile(std::string filename);

	//@param: void
	//@return: void
	//@logic: prints the master list
	void print_BAV();

	void save_BAV(std::string filename);
};