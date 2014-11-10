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
	static void compareThreadEngine(HMMengine &hmmEngine, EngineStatus* engineStatus, std::string &text);
	void compareWithFile(MetaData metaData);
	void create(MetaData metaData);
};

