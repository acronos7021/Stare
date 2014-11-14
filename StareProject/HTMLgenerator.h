#pragma once
#include <vector>
#include <string>


#include "DataStructs.h"

class HTMLgenerator
{
public:
	// DestinationFilename is the output HTML file.
	// rankingList is the list of similarities between the source document and the document database.
	HTMLgenerator(string DestinationFilename, int documentID, vector<SentenceRanking> rankingList);
	~HTMLgenerator();
};

