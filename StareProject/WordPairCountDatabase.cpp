#include <unordered_map>
#include "WordPairCountDatabase.h"
#include "Tokenizer.h"


WordPairCountDatabase::~WordPairCountDatabase()
{
}



void WordPairCountDatabase::AddTokenCount(int currWordToken, int nextWordToken, int StyleID)
{
	//if ((currWordToken < 256) || (nextWordToken < 256))
	//	// check for invalid symbols
	//	if ((Tokenizer::checkIgnore((char)currWordToken)) || (Tokenizer::checkIgnore((char)nextWordToken)))
	//		return;  // eliminate symbols from calculations
	uint_fast64_t wordPair = ((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
	std::unordered_map<uint_fast64_t, WordPairCountStruct>::iterator found = WordPairCount.find(wordPair);

	if (found == WordPairCount.end())
	{
		WordPairCount.insert(std::make_pair(wordPair, WordPairCountStruct(StyleID)));
	}
	else
	{
		found->second.addStyle(StyleID,1);
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


