#pragma once
#include <map>
#include <iomanip>
#include "DataStructs.h"
#include "Tokenizer.h"
#include "HTMLgenerator.h"
#include "DocumentDatabase.h"
#include "EngineStatus.h"


class HMMengine
{
public:
	HMMengine();
	~HMMengine();

	DocumentDatabase dataBase;
	Tokenizer tokenizer;

	int learn(MetaData metaData);
	int learnFromFile(MetaData metaData);
	void compare(MetaData metaData);
	void compareThread(EngineStatus* engineStatus, std::string &text);
	static void compareThreadEngine(HMMengine* hmm, EngineStatus* engineStatus, std::string text);
	void compareWithFile(HMMengine &hmm, MetaData metaData);

	void create(MetaData metaData);
	std::string createDoc(int styleID, int length);
	std::vector<int> createHelper(int styleID, int wordID);
	//functions from wordpair db that are required:
	//need db.getWordN(int styleID, int wordID) , ret vector <data> data (a list of the words that come next)
	//check db.getWordStyleCount(int StyleID), ret the max number of tokens in the db
	//need db.validToken(int styleID, int WordID), returns true or false;f
};

