#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include "WordPairCountDatabase.h"

//struct WordNextCountStruct
//{
//	int wordTokenID;
//	int count;
//	std::vector<int> StyleCounts;
//
//	WordNextCountStruct(int WordTokenID, int StyleID, int set_count)
//	{
//		count = 0;
//		wordTokenID = WordTokenID;
//		addStyle(StyleID, set_count);
//	}
//
//	void addStyle(int styleID, int set_count)
//	{
//		if ((int)StyleCounts.size() <= styleID)
//			StyleCounts.resize(styleID + 1);
//		StyleCounts[styleID] += set_count;
//		count += set_count;
//	}
//};



class TestCreate
{
public:
	TestCreate();
	~TestCreate();

	void doTest();

	bool checkPunctuation(char c);

	int rand();
	std::vector<int> randList;
	int randIndex = 0;

	std::vector<bool> isWordTokenStateTable;
	int isWordTokenStateTableIndex;
	bool isWordToken(int wordID, int styleID);

	int tokenMapSize;
	
	std::vector<std::vector<std::vector<WordNextCountStruct>>> getNextTokenTable;
	std::vector<WordNextCountStruct> getNextToken(int curr, int style);

	std::string rebuildSent(std::vector<int> in);


	std::string createDoc(int styleID, int length);
	std::vector<int> createHelper(int styleID, int wordID);

};

