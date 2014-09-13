#pragma once
#include "Database.h"
#include "DataStructs.h"
#include "Tokenizer.h"
#include "HTMLgenerator.h"


class HMMengine
{
public:
	HMMengine(MetaData metaData);
	~HMMengine();

private:
	int learn(MetaData metaData);
	void compare(MetaData metaData);
	void create(MetaData metaData);
};

