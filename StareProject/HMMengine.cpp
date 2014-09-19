#include "HMMengine.h"


HMMengine::HMMengine(MetaData metaData)
{
	switch (metaData.action)
	{
		case ActionType::Learn:
			learn(metaData);
			break;
		case ActionType::Compare:
			compare(metaData);
			break;
		case ActionType::Create:
			create(metaData);
			break;
	}
}

HMMengine::~HMMengine()
{
}

int HMMengine::learn(MetaData metaData)
{
	// do learn
	Tokenizer tokenizer = Tokenizer(metaData.DocumentText);
	StyleDatabase styleDB;
	vector<string> words;

	// add the metadata to the database.
	bool alreadyExists;
	int documentID = styleDB.insertDocument(alreadyExists, metaData.Author, metaData.Title, metaData.PublishDate);
	// add the document's sentences to the database.
	if (!alreadyExists)
	{
		while ((words = tokenizer.getNextSentence()).size() > 0)
			styleDB.insertSentence(documentID, words);
	}

	return documentID;
}

void HMMengine::compare(MetaData metaData)
{
	vector<sentenceRanking> rankingList;

	// add the document you want to compare to the database.  This is easier than trying
	// to docode it manually.
	int compareDoc = learn(metaData);

	// do my compare thing to get sentence rankings.
	// execut the following as a case of plagerism is discovered.
	rankingList.push_back(sentenceRanking(compareDoc, 2, 14, 0.1));
	rankingList.push_back(sentenceRanking(compareDoc, 8, 33, 0.2));

	// pass to the HTMLgenerator all of the sentences that are found to be similiar 
	// and the information it needs to format the output.
	HTMLgenerator hGen = HTMLgenerator("cmp.html", compareDoc, rankingList);

}

void HMMengine::create(MetaData metaData)
{

}
