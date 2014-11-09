#include <vector>
#include <unordered_map>
#include <cstdint>
#include <memory>
#include <mutex>

#pragma once

struct WordPairCountStruct
{
	int count;  // this is the global count in all styles for this word pair
	std::vector<int> StyleCounts; //Use the style id to index the count
	bool hasChanged;  //

	WordPairCountStruct()
	{
		count = 0;
		hasChanged = true;
	}

	WordPairCountStruct(int styleID)
	{
		count = 0;
		addStyle(styleID, 1);
	}

	//void addStyle(int styleID)
	//{
	//	count++;
	//	StyleCounts[styleID]++;
	//	hasChanged = true;
	//}

	void addStyle(int styleID, int set_count)
	{
		if ((int)StyleCounts.size() <= styleID)
			StyleCounts.resize(styleID + 1);
		StyleCounts[styleID] += set_count;
		count += set_count;
		hasChanged = true;
	}

	void resetChange()
	{
		hasChanged = false;
	}
};


class WordPairCountDatabase
{
public:
	WordPairCountDatabase() {}
	~WordPairCountDatabase();

	void AddTokenCount(int currWordToken, int nextWordToken,int StyleID);
	const WordPairCountStruct getTokenCount(int currWordToken, int nextWordToken);

	void FlushToDatabase();
	void SyncWithDatabase();
	//void AddTotalStyleCount(int StyleID);

private:
	std::unordered_map<uint_fast64_t, WordPairCountStruct> WordPairCount;
};

