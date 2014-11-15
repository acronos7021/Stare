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
	int documentID=0;
	if (documentTokens.size() > 0)
	{
		documentID = dataBase.insertDocument( metaData.Author, metaData.Title, metaData.PublishDate);
		dataBase.insertDocumentText(documentID, documentTokens);
	}
	return documentID;
}

int HMMengine::learn(MetaData metaData)
{
	// Tokenize the document we want to learn
	std::deque<std::vector<int>> documentTokens = tokenizer.tokenizeDoc(metaData.DocumentText);
	int documentID=0;
	// insert it into the database;
	if (documentTokens.size() > 0)
	{
		documentID  = dataBase.insertDocument(metaData.Author, metaData.Title, metaData.PublishDate);
		dataBase.insertDocumentText(documentID, documentTokens);
	}
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



struct PlagerismCalculator
{
	// the sentenceList is composed of each of the words in the sentence that is being compared
	// the other dimension is the list of all sentences where that word pair occurs in the database.
	// sentenceList[SourceSentPos][list of all occurences]
	std::vector<std::vector<int>> sentenceList;
	std::vector<size_t> sentIndex;

	void addWordPair(std::set<int> sentList)
	{
		std::vector<int> newSent;
		for (std::set<int>::iterator it = sentList.begin(); it != sentList.end(); ++it)
		{
			newSent.push_back(*it);
		}
		if (newSent.size() > 0) sentenceList.push_back(newSent);
	}

	// returns the sentences with the closest match.
	std::vector<int> doCalc(HMMengine &hmm)
	{
		std::vector<int> ret;

		// create the indexes
		for (size_t wp = 0; wp < sentenceList.size(); ++wp)
		{
			sentIndex.push_back(0);
		}
		// find the minimums
		bool keepGoing = true;
		int currMin = 0;
		int nextMin = INT_MAX;
		int currTotal = 0;
		while (keepGoing)
		{
			keepGoing = false;
			for (size_t wp = 0; wp < sentenceList.size(); ++wp)
			{
				// Only consider to the end of the list for each word pair.  When all lists are exhausted keepGoing will be false
				//vector<int> compareSent = hmm.dataBase.TotalSentenceList[currMin];
				//std::cout << "compare: " << hmm.tokenizer.rebuildSent(compareSent) << std::endl;
				if (sentIndex[wp] < sentenceList[wp].size())
				{
					// There are still some items left to consider 
					keepGoing = true;
					if (sentenceList[wp][sentIndex[wp]] == currMin)
					{
						currTotal++;
						sentIndex[wp]++;
						if (sentIndex[wp] < sentenceList[wp].size())
							if (sentenceList[wp][sentIndex[wp]] < nextMin)
								nextMin = sentenceList[wp][sentIndex[wp]];
					}
					else if (sentenceList[wp][sentIndex[wp]]< nextMin)
					{
						nextMin = sentenceList[wp][sentIndex[wp]];

					}
				}
			}
			if (currTotal>5) 
				ret.push_back(currMin);
			currTotal = 0;
			currMin = nextMin;
			nextMin = INT_MAX;
		}
		return ret;
	}

	//std::vector<int> compare(std::vector<int> source, std::vector<int> database,int &score)
	//{
	//	score = 0;
	//	vector<int> overlap;
	//	overlap.reserve(source.size()); // defaults to 0 for all positions
	//	for (size_t si = 0; si < source.size(); ++si)
	//	{
	//		for (size_t di = si; di < database.size(); ++di)
	//		{
	//			int di_tmp=di;
	//			while (source[si] == database[di_tmp])
	//			{
	//				di_tmp++;
	//				if (++si == source.size())
	//					break;
	//			}
	//			int offset = di_tmp - di;
	//			if (offset>1)
	//			{
	//				std::fill(overlap[si], overlap[si + offset], 1);
	//			}
	//			if (offset > score) score = offset;
	//		}
	//	}
	//}

	void clear()
	{
		sentenceList.clear();
		std::vector<int> sentIndex;
	}

};




// To help organize the data, one of these objects is kept to hold the calculations for each style as we
// traverse through the document.
struct StyleCalculator
{
    int StyleID;

	int totalStyleCountsInSent;
	int totalCountsInSent;
	int totalStyleCountsInDoc;
	int totalCountsInDoc;
	double weight;

	StyleCalculator(int id)// , double sentProb, double docProb)
    {

		StyleID = id; 
		weight = 1; 

		totalStyleCountsInSent = 0;
		totalCountsInSent=0;
		totalStyleCountsInDoc=0;
		totalCountsInDoc=0;
    }

	void setWeight(double newWeight)
	{
		weight = newWeight;
	}

    void incrementStats(int styleCounts,int totalCounts)
    {
		totalStyleCountsInSent += styleCounts;
		totalCountsInSent += totalCounts;
		totalStyleCountsInDoc += styleCounts;
		totalCountsInDoc += totalCounts;
    }

	double getSentProb()
	{
		return weight * (double)totalStyleCountsInSent / totalCountsInSent;
	}

	double getDocProb()
	{
		return weight * (double)totalStyleCountsInDoc / totalCountsInDoc;
	}

	void resetSentence()
	{
		totalStyleCountsInSent =0;
		totalCountsInSent = 0;
	}
};



void HMMengine::compareThreadEngine(HMMengine &hmm,EngineStatus* engineStatus, std::string &text)
{
    std::deque<std::vector<int>> documentTokens = hmm.tokenizer.tokenizeDoc(text);
	
	PlagerismCalculator plagCalc;

    std::map<int,StyleCalculator> styleCalcs;
    int stylesCount = 0;
    for (size_t st = 0; st < hmm.dataBase.StyleList.size(); ++st)
        if (hmm.dataBase.wpd.getWordStyleCount(st) != 0) {
            stylesCount++;
            styleCalcs.insert(std::pair<int,StyleCalculator>(st, StyleCalculator(st)));
        }
    if (stylesCount<1) return;

	// assign the weight of different styles.  Styles with a larger corpus need a smaller weight.
	for (std::map<int, StyleCalculator>::iterator it = styleCalcs.begin(); it != styleCalcs.end(); ++it)
	{
		int thisStyleID = it->second.StyleID;
		int thisStyleWordCount = hmm.dataBase.wpd.getWordStyleCount(it->second.StyleID);
		int totalWordCount = hmm.dataBase.wpd.getTotalWordCount();
		double weight = (totalWordCount - thisStyleWordCount) / ((double)totalWordCount / stylesCount);
		it->second.setWeight(weight);

	}

	//double WPprob, SentNormProb, DocNormProb;

    for (size_t s = 0; s < documentTokens.size(); s++) {
        size_t sentSize = documentTokens[s].size() - 1;
		// It is a new sentence, so clear the stylcCalc sentence counters.
		std::cout << "source: " << hmm.tokenizer.rebuildSent(documentTokens[s]) << std::endl;
		std::cout << std::endl << "Sent#" << s;
		for (std::map<int, StyleCalculator>::iterator it = styleCalcs.begin(); it != styleCalcs.end(); ++it)
		{
			//// Calculate Sentence Probability
			//WPprob = (double)it->second.totalStyleCountsInSent / it->second.totalCountsInSent; // (double)wordpairstylecount / totalwpc;
			//SentNormProb = WPprob *
			//	(hmm.dataBase.wpd.getTotalWordCount() - hmm.dataBase.wpd.getWordStyleCount(it->second.StyleID)) /
			//	((double)hmm.dataBase.wpd.getTotalWordCount() / stylesCount);
			//// Calculate document Probability
			//WPprob = (double)it->second.totalStyleCountsInDoc / it->second.totalCountsInDoc; // (double)wordpairstylecount / totalwpc;
			//DocNormProb = WPprob *
			//	(hmm.dataBase.wpd.getTotalWordCount() - hmm.dataBase.wpd.getWordStyleCount(it->second.StyleID)) /
			//	((double)hmm.dataBase.wpd.getTotalWordCount() / stylesCount);

			std::cout << std::fixed << std::setprecision(2) 
				<< "   " << hmm.dataBase.StyleList[it->second.StyleID].Author << " " << it->second.getSentProb() << " : " << it->second.getDocProb() 
				<< " (" << it->second.totalStyleCountsInDoc << ":" << it->second.totalCountsInDoc << ") ";
			it->second.resetSentence();
		}

		std::cout << std::endl;

		// Step trough all of the words this sentence.
        for (size_t w = 0; w < sentSize; w++) 
		{
            int prevWordToken, nextWordToken, currWordToken;
            if (hmm.dataBase.getPrevAndNext(s, w, prevWordToken, nextWordToken, documentTokens)) 
			{
				currWordToken = documentTokens[s][w];
				// Should be a valid word token at this time.
				//std::cout << hmm.tokenizer.tdb.GetString(currWordToken) << ":" << hmm.tokenizer.tdb.GetString(nextWordToken) << std::endl;
				plagCalc.addWordPair(hmm.dataBase.wpd.getSentenceList(currWordToken, nextWordToken));
                //std::cout << "Sentence " << s << std::endl;
                for (std::map<int, StyleCalculator>::iterator it = styleCalcs.begin(); it != styleCalcs.end(); ++it) 
				{
                    //std::cout << "How deep does the rabit hole go? " << std::endl;
                    int totalwpc = hmm.dataBase.wpd.getTotalWordPairCount(currWordToken, nextWordToken);
                    if (totalwpc > 0) 
					{
						int wordpairstylecount = hmm.dataBase.wpd.getWordPairStyleCount(currWordToken, nextWordToken, it->second.StyleID);
						it->second.incrementStats(wordpairstylecount, totalwpc);
                        
                        //std::cout << "wordpairstylecount:" << wordpairstylecount << "    totalwpc:" << totalwpc << std::endl;
                        //double WPprob = (double)wordpairstylecount / totalwpc;
                        //double NormProb = WPprob *
                        //        (hmm.dataBase.wpd.getTotalWordCount() - hmm.dataBase.wpd.getWordStyleCount(it->second.StyleID)) /
                        //        ((double)hmm.dataBase.wpd.getTotalWordCount() / stylesCount);
                        //it->second.addToAverage(NormProb);
                        //std::cout << "    WPprob:" << WPprob << "    NormProb:" << NormProb << std::endl;
                        //std::cout << "    style:" << it->second.StyleID
                        //        << "      sentProb:" << it->second.sentProbability << "   DocProb:" << it->second.docProbability << std::endl;
                    }
                }
            }
        }
		vector<int> result = plagCalc.doCalc(hmm);
		if (result.size() > 0)
		{
			std::cout << "Plagerism sentences: ";
			for (vector<int>::iterator i = result.begin(); i != result.end(); ++i)
			{
				std::cout << *i << "," << std::endl;
				std::cout << "database: " << hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[*i]) << std::endl;

			}
		}
			
    }
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


string HMMengine::createDoc(int styleID, int length)
{
	string s = "";
	vector<int> tmp;
	srand(time(0));

	int random;
	int maxTok = tokenizer.tdb.tokenMap.size();

	int count = 0;
	bool lock = false;

	while (count < length && maxTok > 256)
	{
		random = (int)(rand() % maxTok);
		lock = true;
		while (lock)
		{
			if (random <= 256)
			{
				if (random < 128)
				{
					random = 73; // I
				}
				else
				{
					random = 65; // A
				}
			}
			if (dataBase.wpd.isWordToken(random, styleID) == true)
			{
				tmp = createHelper(styleID, random);
				s += tokenizer.rebuildSent(tmp);
				s += " ";
				lock = false;
				count++;
			}
			else
			{
				random = (int)(rand() % maxTok);
			}
		}//end of inner loop
	}//end of outer loop
	return s;
} //end of createDoc2()

vector<int> HMMengine::createHelper(int styleID, int wordID)
{
	int curr = wordID;
	Tokenizer tok;
	vector<int> hold;
	int threshold = 10;
	hold.push_back(curr);
	srand(time(0));

	float random;
	float percent = 0.0;
	int thres = (int)(rand() % 6);
	int max = 0;

	vector<WordNextCountStruct>dataW;
	dataW = dataBase.wpd.getNextToken(curr, styleID);
	for (int i = 0; i < dataW.size(); i++)
	{
		max += dataW[i].count;
	}

	int  ind = 0;
	bool lock = false;

	while (!tok.checkPunctuation((char)(curr)) && thres <= threshold)
	{
		random = (float)(rand() % 101) / 100;
		lock = true;
		ind = 0;
		percent = 0.0;
		while (lock)
		{
			if (dataW.size() >= 1 && ind < dataW.size())
			{
				percent += ((float)dataW[ind].count / (float)max);
				if (random <= percent )//&& dataBase.wpd.isWordToken(random, styleID) == true) Function almost always ret false
				{
					/*if (tok.checkPunctuation((char)(curr)))
					{
						hold.pop_back();
					}*/
					hold.push_back(dataW[ind].wordTokenID);
					hold.push_back((int)'&');
					curr = dataW[ind].wordTokenID;
					lock = false;
				}
				ind++;
			}
			else
			{
				lock = false;
			}
		}//end of inner loop

		max = 0;
		dataW = dataBase.wpd.getNextToken(curr, styleID);
		for (int i = 0; i < dataW.size(); i++)
		{
			max += dataW[i].count;
		}
		thres++;
	}//end of outer loop

	if (thres > threshold)
	{
		hold.pop_back();
		hold.push_back(46); // always a period
	}

	return hold;
}//end of createDocH2()
