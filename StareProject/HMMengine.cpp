#include "HMMengine.h"


HMMengine::HMMengine()
{
	//switch (metaData.action)
	//{
	//	case ActionType::Learn:
	//		learn(metaData);
	//		break;
	//	case ActionType::Compare:
	//		compare(metaData);
	//		break;
	//	case ActionType::Create:
	//		create(metaData);
	//		break;
	//}
}

HMMengine::~HMMengine()
{
}

int HMMengine::learn(MetaData metaData)
{
	// do learn
	//StyleDatabase styleDB = StyleDatabase::getInstance();
	//styleDB.open("AIsql.db3");

	Tokenizer tokenizer = Tokenizer();
	std::vector <std::vector<int>> documentTokens = tokenizer.tokenizeDoc(metaData.DocumentText);
	int documentID = db.insertDocument( metaData.Author, metaData.Title, metaData.PublishDate);
	db.insertDocumentText(documentID, documentTokens);
	return documentID;
}

void HMMengine::compare(MetaData metaData)
{

	Tokenizer tokenizer = Tokenizer();
	std::vector <std::vector<int>> documentTokens = tokenizer.tokenizeDoc(metaData.DocumentText);

	vector<StyleCounts> totalWordCountsPerStyle = db.getTotalWordCountPerStyle();

	map<int, int> pairCountsPerStyle;

	for (int s = 0; s < documentTokens.size(); s++)
	{
		int sentSize = documentTokens[s].size() - 1;
		for (int w = 0; w < sentSize; w++)
		{
			// every word of every sentence except the last word in the sentence should be here.
			vector<StyleCounts> wordCountsPerPath = db.getPathWordCountPerStyle(documentTokens[s][w], documentTokens[s][w + 1]);
			for (int i = 0; i < wordCountsPerPath.size(); i++)
			{
				map<string, int>::iterator it = pairCountsPerStyle.find(wordCountsPerPath);
				if (it == pairCountsPerStyle.end())
			}
		}
	}

	//vector<sentenceRanking> rankingList;

	//// add the document you want to compare to the database.  This is easier than trying
	//// to docode it manually.
	//int compareDoc = learn(metaData);

	//// do my compare thing to get sentence rankings.
	//// execut the following as a case of plagerism is discovered.
	//rankingList.push_back(sentenceRanking(compareDoc, 2, 14, 0.1));
	//rankingList.push_back(sentenceRanking(compareDoc, 8, 33, 0.2));

	//// pass to the HTMLgenerator all of the sentences that are found to be similiar 
	//// and the information it needs to format the output.
	//HTMLgenerator hGen = HTMLgenerator("cmp.html", compareDoc, rankingList);

}

void HMMengine::create(MetaData metaData)
{

}
