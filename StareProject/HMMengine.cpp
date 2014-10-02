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

	map<int, StyleCounter> pairCountsPerStyle;

	//pairCountsPerStyle.insert(std::pair<int, int>(1, 1));
	int sentence = 0;
	for (int s = 0; s < documentTokens.size(); s++)
	{
		int sentSize = documentTokens[s].size() - 1;
		for (int w = 0; w < sentSize; w++)
		{
			// every word of every sentence except the last word in the sentence should be here.
			vector<StyleCounts> wordCountsPerPath = db.getPathWordCountPerStyle(documentTokens[s][w], documentTokens[s][w + 1]);
			for (int i = 0; i < wordCountsPerPath.size(); i++)
			{
				map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(wordCountsPerPath[i].StyleID);
				if (it == pairCountsPerStyle.end())
				{
					// not in the list, so insert it
					pairCountsPerStyle.insert(std::pair<int, StyleCounter>(wordCountsPerPath[i].StyleID, StyleCounter(wordCountsPerPath[i].Count)));
				}
				else
				{
					// already exists so increment it.
					it->second.SentenceCount += wordCountsPerPath[i].Count;
					it->second.TotalCount += wordCountsPerPath[i].Count;

				}
			}
		}
		sentence++;
		cout << endl << "Sentence:" << sentence << endl;

		double totalSentencePercent = 0;
		double totalDocumentPercent = 0;
		for (int i = 0; i < totalWordCountsPerStyle.size(); i++)
		{
			map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(totalWordCountsPerStyle[i].StyleID);
//			double itrSentenceCount = it->second.SentenceCount;
			it->second.SentencePercent = it->second.SentenceCount / (double)totalWordCountsPerStyle[i].Count;
			it->second.TotalDocPercent = it->second.TotalCount / (double)totalWordCountsPerStyle[i].Count;
			totalSentencePercent += it->second.SentencePercent;
			totalDocumentPercent += it->second.TotalDocPercent;
//			cout << totalWordCountsPerStyle[i].Author << " " << it->second.SentenceCount << "-" << it->second.TotalCount << endl;
		}

		for (int i = 0; i < totalWordCountsPerStyle.size(); i++)
		{
			map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(totalWordCountsPerStyle[i].StyleID);
			double sentencePercent = it->second.SentencePercent / totalSentencePercent * 100;
			double docPercent = it->second.TotalDocPercent / totalDocumentPercent * 100;
//			cout << setprecision(2) << totalWordCountsPerStyle[i].Author << " By Sentence:" << it->second.SentenceCount << ":" << sentencePercent << "% Total To Now: " << it->second.TotalCount << ":" << docPercent << "%" << endl;
			cout << setprecision(2) << totalWordCountsPerStyle[i].Author << " => Sentence:" << sentencePercent << "%    Total: "  << docPercent << "%" << endl;
			it->second.SentenceCount = 0;
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
