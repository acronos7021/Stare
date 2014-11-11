
#include "WordPairCountDatabase.h"
#include "Tokenizer.h"



WordPairCountDatabase::~WordPairCountDatabase()
{
}

uint_fast64_t WordPairCountDatabase::getWordPairBitPack(int currWordToken, int nextWordToken)
{
    return ((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
}


// handles all input into this database
void WordPairCountDatabase::AddCounts(int currWordToken,int nextWordToken, int StyleID, int sentenceID)
{
      // makes the assumpion that the function will only be called once for each word in the database.
      // otherwise we need a separate funcion call to handle totalWordCount and wordStyleCount.
      totalWordCount++;  
      if ((int)wordStyleCount.size() <= StyleID)
	      wordStyleCount.resize(StyleID + 1);
      wordStyleCount[StyleID]++;
      
      // handle the ones that require word wordpairs  
      // These are stored in unordered_map of WordPairCountStructs.  Which includes:
      // count,StyleCounts, SentenceLists which are all initialized in the constructor and incremented using addStyle
      uint_fast64_t wordPair = getWordPairBitPack(currWordToken,nextWordToken);//((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
      std::unordered_map<uint_fast64_t, WordPairCountStruct>::iterator found = WordPairCount.find(wordPair);
      if (found == WordPairCount.end())
      {
	  // this wordpair does not exist.  So create it.
	      WordPairCount.insert(std::make_pair(wordPair, WordPairCountStruct(StyleID, sentenceID)));
      }
      else
      {
	      found->second.addStyle(StyleID,1,sentenceID);
      }
}

const int WordPairCountDatabase::getTotalWordCount()
{
    return totalWordCount;
}

const int WordPairCountDatabase::getWordStyleCount(int StyleID)
{
    if ((StyleID>=0)&&(StyleID<wordStyleCount.size()))
      return wordStyleCount[StyleID];
    else
      return 0;
}

const int WordPairCountDatabase::getTotalWordPairCount(int currWordToken , int nextWordToken)
{
      uint_fast64_t wordPair = getWordPairBitPack(currWordToken,nextWordToken);//((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
    std::unordered_map<uint_fast64_t, WordPairCountStruct>::iterator found = WordPairCount.find(wordPair);
    if (found == WordPairCount.end())
    {
	//The pair was not found.  No occurences
	return 0;
    }
    else
    {
	return found->second.count;
    }
}

const int WordPairCountDatabase::getWordPairStyleCount(int currWordToken , int nextWordToken,int StyleID)
{
      uint_fast64_t wordPair = getWordPairBitPack(currWordToken,nextWordToken);//((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
    std::unordered_map<uint_fast64_t, WordPairCountStruct>::iterator found = WordPairCount.find(wordPair);
    if (found == WordPairCount.end())
    {
	//The pair was not found.  No occurences
	return 0;
    }
    else
    {
	if ((StyleID>=0)&&(StyleID<wordStyleCount.size()))
	    return found->second.StyleCounts[StyleID];
	else
	    return 0;  //the style was not found.  No occurences
	
    }
}

const std::set<int> WordPairCountDatabase::getSentenceList(int currWordToken, int nextWordToken)
{
      uint_fast64_t wordPair = getWordPairBitPack(currWordToken,nextWordToken);//((uint_fast64_t)currWordToken << 32) + (uint_fast64_t)nextWordToken;
    std::unordered_map<uint_fast64_t, WordPairCountStruct>::iterator found = WordPairCount.find(wordPair);
    if (found == WordPairCount.end())
    {
	//The pair was not found
	//vector<int> emptyvect;
	return std::set<int>();//emptyvect;
    }
    else
    {
	return found->second.SentenceList;
	
    }
}

void WordPairCountDatabase::clearAll()
{
  totalWordCount = 0;
  wordStyleCount.clear();
  WordPairCount.clear();
}


