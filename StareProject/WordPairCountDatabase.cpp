#include <unordered_map>
#include "WordPairCountDatabase.h"


WordPairCountDatabase::~WordPairCountDatabase()
{
}



void WordPairCountDatabase::AddTokenCount(int currWordToken, int nextWordToken, int StyleID)
{
	uint_fast64_t wordPair = ((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
	std::unordered_map<uint_fast64_t, WordPairCountStruct>::iterator found = WordPairCount.find(wordPair);
	if (found == WordPairCount.end())
	{
		WordPairCount.insert(std::make_pair(wordPair, WordPairCountStruct(StyleID)));
	}
	else
	{
		found->second.addStyle(StyleID);
	}
}

const WordPairCountStruct WordPairCountDatabase::getTokenCount(int currWordToken, int nextWordToken)
{
	uint_fast64_t wordPair = ((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
	std::unordered_map<uint_fast64_t, WordPairCountStruct>::iterator found = WordPairCount.find(wordPair);
	if (found == WordPairCount.end())
		return WordPairCountStruct();
	else
		return found->second;
}

void WordPairCountDatabase::FlushToDatabase()
{

}

void WordPairCountDatabase::SyncWithDatabase()
{

}


