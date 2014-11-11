#include "HMMengine.h"
#include <thread>


HMMengine::HMMengine()
{
	//dataBase = db;
	//tokenizer = tokenizer_in;
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

int HMMengine::learnFromFile(MetaData metaData)
{
	// Tokenize the document we want to learn
	std::deque<std::vector<int>> documentTokens = tokenizer.tokenizeFile(metaData.DocumentText);
	//int testint = documentTokens.size();
	//std::cout << metaData.DocumentText;
	// insert it into the database;
	int documentID = dataBase.insertDocument( metaData.Author, metaData.Title, metaData.PublishDate);
	dataBase.insertDocumentText(documentID, documentTokens);
	return documentID;
}

int HMMengine::learn(MetaData metaData)
{
	// Tokenize the document we want to learn
	std::deque<std::vector<int>> documentTokens = tokenizer.tokenizeDoc(metaData.DocumentText);

	// insert it into the database;
	int documentID = dataBase.insertDocument(metaData.Author, metaData.Title, metaData.PublishDate);
	dataBase.insertDocumentText(documentID, documentTokens);
	return documentID;
}

void HMMengine::compareWithFile(HMMengine &hmm, MetaData metaData)
{
	// This is due to an early misunderstanding between Sam and Brian.  Should be fixed.
	std::string documentText = tokenizer.readFile(metaData.DocumentText);
        //std::thread t(hmm.compareThreadEngine, hmm, engineStatus, text);
	EngineStatus empty(0);
	compareThreadEngine(hmm,&empty,documentText);
}
//HMMengine &hmm, 


void HMMengine::compareThreadEngine(HMMengine &hmm,EngineStatus* engineStatus, std::string &text)
{
}
/*

	std::deque<std::vector<int>> documentTokens = hmm.tokenizer.tokenizeDoc(text);

	vector<StyleCounts> totalWordCountsPerStyle = hmm.dataBase.getTotalWordCountPerStyle();

	map<int, StyleCounter> pairCountsPerStyle;

	int sentence = 0;
	for (int s = 0; s < (int)documentTokens.size(); s++)
	{
		int sentSize = documentTokens[s].size() - 1;
		for (int w = 0; w < sentSize; w++)
		{
			// every word of every sentence except the last word in the sentence should be here.
			// This does a compare of each pair with the database.  It increments a count and loads it into a map for each pair found.
			int prevWordToken, nextWordToken;
			if (hmm.dataBase.getPrevAndNext(s, w, prevWordToken, nextWordToken, documentTokens))
			{
				// this is a WordToken and nextWordToken has been calculated.
				vector<StyleCounts> wordCountsPerPath = hmm.dataBase.getPathWordCountPerStyle(documentTokens[s][w], nextWordToken);
				for (unsigned int i = 0; i < wordCountsPerPath.size(); i++)
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
		}
		sentence++;
		cout << endl << "Sentence:" << sentence << endl;

		// Calculate the totals which are printed out in the next section.
		// we need two loops because current/total needs total to print.
		double totalSentencePercent = 0;
		double totalDocumentPercent = 0;
		for (unsigned int i = 0; i < totalWordCountsPerStyle.size(); i++)
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
		for (unsigned int i = 0; i < totalWordCountsPerStyle.size(); i++)
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
*/


//void HMMengine::compare(MetaData metaData)
//{
//	std::deque<std::vector<int>> documentTokens = tokenizer.tokenizeFile(metaData.DocumentText);
//
//	vector<StyleCounts> totalWordCountsPerStyle = dataBase.getTotalWordCountPerStyle();
//
//	map<int, StyleCounter> pairCountsPerStyle;
//
//	int sentence = 0;
//	for (int s = 0; s < (int)documentTokens.size(); s++)
//	{
//		int sentSize = documentTokens[s].size() - 1;
//		for (int w = 0; w < sentSize; w++)
//		{
//			// every word of every sentence except the last word in the sentence should be here.
//			// This does a compare of each pair with the database.  It increments a count and loads it into a map for each pair found.
//			int prevWordToken, nextWordToken;
//			if (dataBase.getPrevAndNext(s, w, prevWordToken, nextWordToken, documentTokens))
//			{
//				// this is a WordToken and nextWordToken has been calculated.
//				vector<StyleCounts> wordCountsPerPath = dataBase.getPathWordCountPerStyle(documentTokens[s][w], nextWordToken);
//				for (unsigned int i = 0; i < wordCountsPerPath.size(); i++)
//				{
//					map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(wordCountsPerPath[i].StyleID);
//					if (it == pairCountsPerStyle.end())
//					{
//						// not in the list, so insert a hit counter with the current count.
//						pairCountsPerStyle.insert(std::pair<int, StyleCounter>(wordCountsPerPath[i].StyleID, StyleCounter(wordCountsPerPath[i].Count)));
//					}
//					else
//					{
//						// already exists so increment the hits for that style.
//						it->second.SentenceCount += wordCountsPerPath[i].Count;
//						it->second.TotalCount += wordCountsPerPath[i].Count;
//
//					}
//				}
//			}
//		}
//		sentence++;
//		cout << endl << "Sentence:" << sentence << endl;
//
//		// Calculate the totals which are printed out in the next section.
//		// we need two loops because current/total needs total to print.
//		double totalSentencePercent = 0;
//		double totalDocumentPercent = 0;
//		for (unsigned int i = 0; i < totalWordCountsPerStyle.size(); i++)
//		{
//			// examines each style.  If the style has hits, it tracks that and uses it in the totals.
//			map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(totalWordCountsPerStyle[i].StyleID);
//			if (it != pairCountsPerStyle.end())
//			{
//				it->second.SentencePercent = it->second.SentenceCount / (double)totalWordCountsPerStyle[i].Count;
//				it->second.TotalDocPercent = it->second.TotalCount / (double)totalWordCountsPerStyle[i].Count;
//				totalSentencePercent += it->second.SentencePercent;
//				totalDocumentPercent += it->second.TotalDocPercent;
//			}
//			else
//			{
//				// the sentence did not have any hits for this style.
//			}
//		}
//
//		// now print out the results.
//		for (unsigned int i = 0; i < totalWordCountsPerStyle.size(); i++)
//		{
//			map<int, StyleCounter>::iterator it = pairCountsPerStyle.find(totalWordCountsPerStyle[i].StyleID);
//			if (it != pairCountsPerStyle.end())
//			{
//				double sentencePercent = it->second.SentencePercent / totalSentencePercent * 100;
//				double docPercent = it->second.TotalDocPercent / totalDocumentPercent * 100;
//				cout << setprecision(2) << totalWordCountsPerStyle[i].Author << " => Sentence:" << sentencePercent << "%    Total: " << docPercent << "%" << endl;
//				it->second.SentenceCount = 0;
//			}
//			else
//			{
//				// the sentence did not have any hits for this style.
//			}
//		}
//		
//	}
//}


//vector<SentenceRanking> plagerizedSentences;

//SentenceBlob sbSource;
//sbSource.NextNextSentenceStr = "Sentence 9";
//sbSource.NextSentenceStr = "Sentence 8";
//sbSource.SentenceStr = "Sentence 7";
//sbSource.PrevSentenceStr = "Sentence 6";
//sbSource.PrevPrevSentenceStr = "Sentence 5";
//sbSource.SentenceID = 7;

//SentenceBlob sbFound;
//sbFound.NextNextSentenceStr = "Sentence 9";
//sbFound.NextSentenceStr = "Sentence 8";
//sbFound.SentenceStr = "Sentence 7";
//sbFound.PrevSentenceStr = "Sentence 6";
//sbFound.PrevPrevSentenceStr = "Sentence 5";
//sbFound.SentenceID = 7;

//test.push_back(SentenceRanking("Dickens", "A tale of two cities", sbSource, sbFound, .57));
//test.push_back(SentenceRanking("Mark Twain", "Huckleberry Finn", sbSource, sbFound, .57));

//StyleCertaintyItem style1;
//style1.certainty = .73;
//style1.StyleID = 2;
//style1.StyleName = "Mark Twain";

//vector<StyleCertaintyItem> ci;
//ci.push_back(style1);

//CompareResult cr;
//cr.documentCertainties = ci;
//cr.sentenceRankings = test;

//return cr;


void HMMengine::create(MetaData metaData)
{

}
