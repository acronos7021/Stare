#include "Database.h"


StyleDatabase::StyleDatabase(void)
{
}


StyleDatabase::~StyleDatabase(void)
{
}




//Use this to create a new Document
//  if a Style (author) doesn’t exist and add it to the Style table.  
//returns the DocumentID of the new Document.
int StyleDatabase::insertDocument(string Author, string Title, string publishDate)
{
	return 0;
}

// Used to add a sentence
// This would:
//	1) convert the vector of words to a vector of integer tokens
//	2) add any new words to the Token table(if needed)
//	3) add these tokens to the Sentence table creating a new row.
//	4) add these tokens to the HMMtokenPath table (along with the StyleID (derived from DocumentID) 
//     and SentenceID (which is remembered from #3).
//     Each word would be a row in the HMMtokenPath table along with the word before it and the word after it.
//	5) Returns the new sentenceID. 
int StyleDatabase::insertSentence(int DocumentID, vector<string> words)
{
	return 0;
}

// Returns the string of the selected sentence. 
string StyleDatabase::getSentence(int sentenceID)
{
	return "";
}

// returns the number of instances (the count) in all documents of that series of 3 words.
//	Returns a list of the SentenceID’s for every occurance of that string. 
int StyleDatabase::getWordGroupCount(string prevWord, string currWord, string nextWord)
{
	return 0;
}

//	same as above but restricts the count to a specific style.
int StyleDatabase::getWordGroupCountByStyle(int StyleID, string prevWord, string currWord, string nextWord)
{
	//		 getWordGroupCountByStyle() / getWordGroupCount() = the percentage of that word group by style.
	return 0;
}

//Returns a list of all of the SentenceIDs where that combination of words exists in all documents of the selected style. 
vector<int> StyleDatabase::getWordGroupListByStyle(int StyleID, string prevWord, string currWord, string nextWord)
{
	vector<int> tokens;
	return tokens;
}
