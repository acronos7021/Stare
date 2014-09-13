#pragma once
#include <vector>
#include <string>
#include "Database.h"
#include "DataStructs.h"

class HTMLgenerator
{
public:
	// DestinationFilename is the output HTML file.
	// rankingList is the list of similarities between the source document and the document database.
	HTMLgenerator(string DestinationFilename, vector<sentenceRanking> rankingList );
	~HTMLgenerator();
};

