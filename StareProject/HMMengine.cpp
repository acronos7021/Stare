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
	// Tokenize the document we want to learn
	Tokenizer tokenizer = Tokenizer();
	std::vector <std::vector<int>> documentTokens = tokenizer.tokenizeDoc(metaData.DocumentText);

	// insert it into the database;
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

	int sentence = 0;
	for (int s = 0; s < documentTokens.size(); s++)
	{
		int sentSize = documentTokens[s].size() - 1;
		for (int w = 0; w < sentSize; w++)
		{
			// every word of every sentence except the last word in the sentence should be here.
			// This does a compare of each pair with the database.  It increments a count and loads it into a map for each pair found.
			vector<StyleCounts> wordCountsPerPath = db.getPathWordCountPerStyle(documentTokens[s][w], documentTokens[s][w + 1]);
			for (int i = 0; i < wordCountsPerPath.size(); i++)
			{
				map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(wordCountsPerPath[i].StyleID);
				if (it == pairCountsPerStyle.end())
				{
					// not in the list, so insert a hit counter with the current count.
					pairCountsPerStyle.insert(std::pair<int, StyleCounter>(wordCountsPerPath[i].StyleID, StyleCounter(wordCountsPerPath[i].Count)));
				}
				else
				{
					// already exists so increment the hits for that style.
					it->second.SentenceCount += wordCountsPerPath[i].Count;
					it->second.TotalCount += wordCountsPerPath[i].Count;

				}
			}
		}
		sentence++;
		cout << endl << "Sentence:" << sentence << endl;

		// Calculate the totals which are printed out in the next section.
		// we need two loops because current/total needs total to print.
		double totalSentencePercent = 0;
		double totalDocumentPercent = 0;
		for (int i = 0; i < totalWordCountsPerStyle.size(); i++)
		{
			// examines each style.  If the style has hits, it tracks that and uses it in the totals.
			map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(totalWordCountsPerStyle[i].StyleID);
			if (it != pairCountsPerStyle.end())
			{
				it->second.SentencePercent = it->second.SentenceCount / (double)totalWordCountsPerStyle[i].Count;
				it->second.TotalDocPercent = it->second.TotalCount / (double)totalWordCountsPerStyle[i].Count;
				totalSentencePercent += it->second.SentencePercent;
				totalDocumentPercent += it->second.TotalDocPercent;
			}
			else
			{
				// the sentence did not have any hits for this style.
			}
		}

		// now print out the results.
		for (int i = 0; i < totalWordCountsPerStyle.size(); i++)
		{
			map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(totalWordCountsPerStyle[i].StyleID);
			if (it != pairCountsPerStyle.end())
			{
				double sentencePercent = it->second.SentencePercent / totalSentencePercent * 100;
				double docPercent = it->second.TotalDocPercent / totalDocumentPercent * 100;
				cout << setprecision(2) << totalWordCountsPerStyle[i].Author << " => Sentence:" << sentencePercent << "%    Total: " << docPercent << "%" << endl;
				it->second.SentenceCount = 0;
			}
			else
			{
				// the sentence did not have any hits for this style.
			}
		}
	}
}

void HMMengine::create(MetaData metaData)
{

}
