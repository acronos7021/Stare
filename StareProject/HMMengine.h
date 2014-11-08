#pragma once
#include <map>
#include <iomanip>
#include "Database.h"
#include "DataStructs.h"
#include "Tokenizer.h"
#include "HTMLgenerator.h"
#include "DocumentDatabase.h"


class HMMengine
{
public:
	HMMengine();
	~HMMengine();

	DocumentDatabase dataBase;
	Tokenizer tokenizer;

	int learn(MetaData metaData);
	void compare(MetaData metaData);
	void create(MetaData metaData);
};

