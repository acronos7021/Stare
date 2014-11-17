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
		std::vector<size_t> sentIndex; // the index of the selected word pair list
		sentIndex.resize(sentenceList.size());

		int biggest_chain = 0;

		std::vector<int> sortedWordPairs;

		// setup the search array.  Each item in the array represents one word pair and contain
		// an integer value of the index in sentenceList that they represent.  These are kept
		// in sorted order and the processing is always done on the smallest item.
		sortedWordPairs.resize(sentenceList.size()+1);
		sortedWordPairs[sentenceList.size()] = -1;
		int i,newItem,tmpItem,chain;

		for (i = 0; i < sortedWordPairs.size(); ++i)
		{
			sortedWordPairs[i] = i;
		}

		for (size_t wp = 0; wp < sentenceList.size(); ++wp)
		{
			i = wp;
			newItem = sentenceList[wp][0];
			while (i>0)
			{
				if (newItem < sentenceList[sortedWordPairs[i-1]][0])
				{
					sortedWordPairs[i] = sortedWordPairs[i - 1];
					--i;
				}
				else
					break;
			}
			sortedWordPairs[i] = wp;
		}

		// search 
		while (sortedWordPairs[0] != -1) // search until the list is empty
		{
			i = 0;
			chain = 0;
			while ((sortedWordPairs[i + 1] != -1) && (sentenceList[sortedWordPairs[i]][sentIndex[sortedWordPairs[i]]] == sentenceList[sortedWordPairs[i + 1]][sentIndex[sortedWordPairs[i + 1]]]))
			{
				++chain;
				++i;
			}
			if (chain > biggest_chain)
				biggest_chain = chain;
			if (chain > 5)
				ret.push_back(sentenceList[sortedWordPairs[i]][0]);

			while (sentenceList[sortedWordPairs[0]][sentIndex[sortedWordPairs[0]]] <= sentenceList[sortedWordPairs[1]][sentIndex[sortedWordPairs[1]]])
			{
				++sentIndex[sortedWordPairs[0]];
				if (sentIndex[sortedWordPairs[0]] >= sentenceList[sortedWordPairs[0]].size())
				{
					// we have reached the end of this word pair chain, so delete it from the list and add -1 at the end
					i = 0;
					while (sortedWordPairs[i] != -1)
					{
						sortedWordPairs[i] = sortedWordPairs[i + 1];
						++i;
					}
				}
			}
			// see if there is only one word pair left.
			if (sortedWordPairs[1] == -1)
				return ret;
			// list is out of order, so resort list
			i = 0;
			while ((sortedWordPairs[i + 1] != -1) && (sentenceList[sortedWordPairs[i]][sentIndex[sortedWordPairs[i]]] > sentenceList[sortedWordPairs[i + 1]][sentIndex[sortedWordPairs[i + 1]]]))
			{
				// swap items
				tmpItem = sortedWordPairs[i];
				sortedWordPairs[i] = sortedWordPairs[i + 1];
				sortedWordPairs[i + 1] = tmpItem;
				++i;
			}
		}



		//// create the indexes
		//for (size_t wp = 0; wp < sentenceList.size(); ++wp)
		//{
		//	sentIndex.push_back(0);
		//}
		//// find the minimums
		//bool keepGoing = true;
		//int currMin = 0;
		//int nextMin = INT_MAX;
		//int currTotal = 0;
		//while (keepGoing)
		//{
		//	keepGoing = false;
		//	for (size_t wp = 0; wp < sentenceList.size(); ++wp)
		//	{
		//		// Only consider to the end of the list for each word pair.  When all lists are exhausted keepGoing will be false
		//		//vector<int> compareSent = hmm.dataBase.TotalSentenceList[currMin];
		//		//std::cout << "compare: " << hmm.tokenizer.rebuildSent(compareSent) << std::endl;
		//		if (sentIndex[wp] < sentenceList[wp].size())
		//		{
		//			// There are still some items left to consider 
		//			keepGoing = true;
		//			if (sentenceList[wp][sentIndex[wp]] == currMin)
		//			{
		//				currTotal++;
		//				sentIndex[wp]++;
		//				if (sentIndex[wp] < sentenceList[wp].size())
		//					if (sentenceList[wp][sentIndex[wp]] < nextMin)
		//						nextMin = sentenceList[wp][sentIndex[wp]];
		//			}
		//			else if (sentenceList[wp][sentIndex[wp]]< nextMin)
		//			{
		//				nextMin = sentenceList[wp][sentIndex[wp]];

		//			}
		//		}
		//	}
		//	if (currTotal>5) 
		//		ret.push_back(currMin);
		//	currTotal = 0;
		//	currMin = nextMin;
		//	nextMin = INT_MAX;
		//}
		//sentenceList.clear();
		return ret;
	}

	std::vector<int> compare(std::vector<int> source, std::vector<int> database,int &score)
	{
		score = 0;
		vector<int> overlap;
		overlap.resize(source.size()); // defaults to 0 for all positions
		for (size_t si = 0; si < source.size(); ++si)
		{
			for (size_t di = 0; di < database.size(); ++di)
			{
				int si_temp = si;
				int di_tmp=di;
				while (source[si_temp] == database[di_tmp])
				{
					di_tmp++;
					if (++si_temp == source.size())
						break;
				}
				int offset = di_tmp - di;
				if (offset>1)
				{
					//std::fill(overlap[si], overlap[si + offset], 1);
					for (int i = si; i<si + offset; ++i)
					{
						overlap[i] = 1;
					}
				}
				si = si_temp;
				if (offset > score) score = offset;
				if (si >= source.size()) break;
			}
		}
		vector<int> ret;
		size_t si = 0;
		bool in_plagerism = false;
		for (size_t i = 0; i < overlap.size(); ++i)
		{
			if (in_plagerism)
			{
				ret.push_back(source[i]);
				//Currently inside a plagerized spot
				if (overlap[i] == 0)
				{
					//exiting plagerized spot.
					ret.push_back(']');
					in_plagerism = false;
				}
			}
			else
			{
				//Currently outside plagerized spot
				if (overlap[i] == 1)
				{
					// entering plagerized spot
					ret.push_back('[');
					in_plagerism = true;
				}
				ret.push_back(source[i]);
			}
		}
		if (in_plagerism)
			ret.push_back(']');

		return ret;
	}

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

SentenceRanking createSentenceRanking(HMMengine &hmm, std::deque<std::vector<int>> &sourceDoc, vector<int> plagarizedSentence, int sourceID, int dbID, int score)
{
	SentenceBlob source;
	source.SentenceID = sourceID;
	source.SentenceStr = hmm.tokenizer.rebuildSent(plagarizedSentence);
	if (sourceID > 1)
		source.PrevSentenceStr = hmm.tokenizer.rebuildSent(sourceDoc[sourceID - 1]);
	else
		source.PrevSentenceStr = "";

	if (sourceID > 2)
		source.PrevPrevSentenceStr = hmm.tokenizer.rebuildSent(sourceDoc[sourceID - 2]);
	else
		source.PrevPrevSentenceStr = "";

	if (sourceID + 1 < sourceDoc.size())
		source.NextSentenceStr = hmm.tokenizer.rebuildSent(sourceDoc[sourceID + 1]);
	else
		source.NextSentenceStr = "";

	if (sourceID + 2 < sourceDoc.size())
		source.NextNextSentenceStr = hmm.tokenizer.rebuildSent(sourceDoc[sourceID + 2]);
	else
		source.NextNextSentenceStr = "";


	SentenceBlob databaseBlob;
	int docID = hmm.dataBase.GetDocIDfromSentID(dbID);
	int sentDocStart, sentDocEnd;
	std::string styleStr, titleStr;
	if (docID >= 0)
	{
		sentDocStart = hmm.dataBase.documentList[docID].startSentenceID;
		sentDocEnd = hmm.dataBase.documentList[docID].endSentenceID;
		styleStr = hmm.dataBase.documentList[docID].Author;
		titleStr = hmm.dataBase.documentList[docID].Title;
		databaseBlob.SentenceStr = hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[dbID]);

		if (dbID - 1 >= sentDocStart)
			databaseBlob.PrevSentenceStr = hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[dbID - 1]);
		else
			databaseBlob.PrevSentenceStr = "";

		if (dbID - 2 >= sentDocStart)
			databaseBlob.PrevPrevSentenceStr = hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[dbID - 2]);
		else
			databaseBlob.PrevPrevSentenceStr = "";

		if (dbID + 1 < sentDocEnd)
			databaseBlob.NextSentenceStr = hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[dbID + 1]);
		else
			databaseBlob.NextSentenceStr = "";

		if (dbID + 2 < sentDocEnd)
			databaseBlob.NextNextSentenceStr = hmm.tokenizer.rebuildSent(hmm.dataBase.TotalSentenceList[dbID + 2]);
		else
			databaseBlob.NextNextSentenceStr = "";
	}
	else
	{
		sentDocStart = 0;
		sentDocEnd = 0;
		styleStr = "";
		titleStr = "";
		databaseBlob.SentenceStr = "";
		databaseBlob.PrevSentenceStr = "";
		databaseBlob.PrevPrevSentenceStr = "";
		databaseBlob.NextSentenceStr = "";
		databaseBlob.NextNextSentenceStr = "";
	}

	SentenceRanking sr(styleStr, titleStr, source, databaseBlob, score);
	return sr;
}


void HMMengine::compareThreadEngine(HMMengine &hmm,EngineStatus* engineStatus, std::string &text)
{
	CompareResult cr;


    std::deque<std::vector<int>> documentTokens = hmm.tokenizer.tokenizeDoc(text);

	engineStatus->setPercentComplete(5);

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
	int progressBump = documentTokens.size() / 90;
	//int progressCount = 0;

    for (size_t s = 0; s < documentTokens.size(); s++) {
        size_t sentSize = documentTokens[s].size() - 1;
		// It is a new sentence, so clear the stylcCalc sentence counters.
		//std::cout << "source: " << hmm.tokenizer.rebuildSent(documentTokens[s]) << std::endl;
		//std::cout << std::endl << "Sent#" << s;
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

			//std::cout << std::fixed << std::setprecision(2) 
			//	<< "   " << hmm.dataBase.StyleList[it->second.StyleID].Author << " " << it->second.getSentProb() << " : " << it->second.getDocProb() 
			//	<< " (" << it->second.totalStyleCountsInDoc << ":" << it->second.totalCountsInDoc << ") ";
			it->second.resetSentence();
		}

		//std::cout << std::endl;

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
		vector<int> result;// = plagCalc.doCalc(hmm);
		if (result.size() > 0)
		{
			int score;
			for (vector<int>::iterator i = result.begin(); i != result.end(); ++i)
			{
				vector<int> plagerized = plagCalc.compare(documentTokens[s], hmm.dataBase.TotalSentenceList[*i], score);
				SentenceRanking newRank = createSentenceRanking(hmm, documentTokens, plagerized, s, *i,score);
				cr.sentenceRankings.push_back(newRank);


				std::cout << "source: " << hmm.tokenizer.rebuildSent(documentTokens[s]) << std::endl;
				std::cout << std::endl << "Sent#" << s << std::endl;

				std::cout << *i << ", score =>" << score << std::endl;
				std::cout << "database: " << hmm.tokenizer.rebuildSent(plagerized) << std::endl;
			}

			//std::cout << "Plagerism sentences: ";
			//int score;
			//int best_score=0;
			//for (vector<int>::iterator i = result.begin(); i != result.end(); ++i)
			//{
			//	vector<int> plagerized = plagCalc.compare(documentTokens[s], hmm.dataBase.TotalSentenceList[*i], score);

			//	std::cout << *i << ", score =>" << score << std::endl;
			//	std::cout << "database: " << hmm.tokenizer.rebuildSent(plagerized) << std::endl;
			//}
		}
		// end sentence processing so update the progress bar
		//++progressCount;
		if ((s % progressBump)==0)
		{ 
			int newPercent = s / progressBump + 5;
			std::cout << "percentComplete:" << newPercent << "%" << std::endl;
			engineStatus->setPercentComplete(newPercent);
		}
    }
	engineStatus->setResult(cr);
	engineStatus->setPercentComplete(100);
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
	srand(time(0)*1.239857128012);

	int random;
	int prev = 0;
	int maxTok = tokenizer.tdb.tokenMap.size();

	int count = 0;
	bool lock = false;

	while (count < length && maxTok > 256)
	{
		random = (int)(rand() % maxTok);
		if (prev == random)
		{
			lock = false;
		}
		else
		{
			prev = random;
			lock = true;
		}
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
				lock = false;
				//random = (int)(rand() % maxTok);
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
		random = (float)(rand() % 100) / 100;
		lock = true;
		ind = 0;
		percent = 0.0;
		while (lock)
		{
			if (dataW.size() >= 1 && ind < dataW.size())
			{
				percent += ((float)dataW[ind].count / (float)max);
				if (random <= percent )//&& dataBase.wpd.isWordToken(random, styleID) == true)// Function almost always ret false
				{
					hold.push_back(dataW[ind].wordTokenID);
					hold.push_back((int)('&'));
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
		hold.push_back(46); // always a period
	}

	return hold;
}//end of createDocH2()
