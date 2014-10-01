#pragma once
#include "Database.h"
#include "DataStructs.h"
#include "Tokenizer.h"
#include "HTMLgenerator.h"


class HMMengine
{
public:
	HMMengine();
	~HMMengine();

	int learn(MetaData metaData);
	void compare(MetaData metaData);
	void create(MetaData metaData);
};

