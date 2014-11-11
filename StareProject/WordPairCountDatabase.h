#include <vector>
#include <set>
#include <unordered_map>
#include <cstdint>
#include <memory>
#include <mutex>

#pragma once

struct WordPairCountStruct
{
	int count;  // this is the global count in all styles for this word pair
	std::vector<int> StyleCounts; //Use the style id to index the count
	std::set<int> SentenceList; // list of the sentence number of every occurence in this database.

	WordPairCountStruct()
	{
		count = 0;
	}

	WordPairCountStruct(int styleID, int sentenceID)
	{
		count = 0;
		addStyle(styleID, 1, sentenceID);
	}

	//void addStyle(int styleID)
	//{
	//	count++;
	//	StyleCounts[styleID]++;
	//	hasChanged = true;
	//}

	void addStyle(int styleID, int set_count, int sentenceID)
	{
	      if ((int)StyleCounts.size() <= styleID)
		      StyleCounts.resize(styleID + 1);
	      StyleCounts[styleID] += set_count;
	      count += set_count;
	      SentenceList.insert(sentenceID);
	}

};

// This database is not stored anywhere on the hard drive.  It is calculated dynamically
// when the system starts and as documents are uploaded.
class WordPairCountDatabase
{
public:
	WordPairCountDatabase() {}
	~WordPairCountDatabase();

	// handles all input into this database
	void AddCounts(int currWordToken,int nextWordToken, int StyleID, int sentenceID);

	
	// total count of all words in all documments
	const int getTotalWordCount();
	// total count of all words for each style.
	const int getWordStyleCount(int StyleID);
	// total count of all words of a specific pair
	const int getTotalWordPairCount(int currWordToken , int nextWordToken);
	// total count of all words of a specific pair and specific style.
	const int getWordPairStyleCount(int currWordToken , int nextWordToken,int StyleID);

	const std::set<int> getSentenceList(int currWordToken, int nextWordToken);
	void clearAll();

private:

	uint_fast64_t getWordPairBitPack(int currWordToken, int nextWordToken);
	std::unordered_map<uint_fast64_t, WordPairCountStruct> WordPairCount;
	std::vector<int>  wordStyleCount; // the count of all words in each style
	int totalWordCount;  // the count of all words in all documents
};

