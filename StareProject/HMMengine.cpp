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

	int documentID = styleDB.insertDocument(metaData.Author, metaData.Title, metaData.PublishDate);
	styleDB.insertSentence(documentID, tokenizer.getNextSentence());

	return documentID;
}

void HMMengine::compare(MetaData metaData)
{
	vector<sentenceRanking> rankingList;

	int compareDoc = learn(metaData);

	// do my compare thing to get sentence rankings.
	rankingList.push_back(sentenceRanking(compareDoc, 2, 14));
	rankingList.push_back(sentenceRanking(compareDoc, 8, 33));

	// pass to the HTMLgenerator all of the sentences that are found to be similiar 
	HTMLgenerator hGen = HTMLgenerator("cmp.html", rankingList);

}

void HMMengine::create(MetaData metaData)
{

}
