#pragma once
using namespace std;

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
//#include "Database.h"


enum ActionType {Learn, Compare, Create};


struct MetaData
{
	ActionType action;
    string Author;
    string Title;
    string PublishDate;
    string DocumentText;

	MetaData(){}

	MetaData(string in_author, string in_title, string in_PublishDate, string in_DocumentText)
	{
		Author = in_author;
		Title = in_title;
		PublishDate = in_PublishDate;
		DocumentText = in_DocumentText;

	}
};
//
//struct sentenceRanking
//{
//	int sentenceID;		  // The selected sentence in the document that is being compared.
//	int foundDocumentID;  // the document id on the server that matches the new source sentence.
//	int foundSentenceID;  // The sentence id on the server that most closely matches the source sentence.
//	double certainty;	  // The certainty that a certain sentence is the particular style listed.
//
//	sentenceRanking(int SentenceID, int FoundDocumentID, int FoundSentenceID, double Certainty)
//	{
//		sentenceID = SentenceID;
//		foundDocumentID = FoundDocumentID;
//		foundSentenceID = FoundSentenceID;
//		certainty = Certainty;
//	}
//};



// the text of the area around a found sentence
struct SentenceBlob
{
	int SentenceID;  // The sentence id on the server that most closely matches the source sentence.
	string PrevPrevSentenceStr;
	string PrevSentenceStr;
	string SentenceStr;
	string NextSentenceStr;
	string NextNextSentenceStr;
	
	void print()
	{
	    std::cout << "sentenceID #" << SentenceID << std::endl;
	    std::cout << "pp: " << PrevPrevSentenceStr<< std::endl;
	    std::cout << "p: " << PrevSentenceStr<< std::endl;
	    std::cout << "o: " << SentenceStr<< std::endl;
	    std::cout << "n: " << NextSentenceStr<< std::endl;
	    std::cout << "nn: " << NextNextSentenceStr<< std::endl;
	}
};

// The comparison results for a single sentence.
struct SentenceRanking
{
	SentenceBlob sourceSentences;
	string foundDocumentStyle;  // the document id on the server that matches the new source sentence.
	string foundDocumentName; // the name of the document on the server tha matches the new source sentence
	SentenceBlob foundSentences;
	double certainty;	  // The certainty that a certain sentence is the particular style listed.

	SentenceRanking(string FoundDocumentStyle, string FoundDocumentName, SentenceBlob SourceSentences, SentenceBlob FoundSentences, double Certainty)
	{
		foundDocumentStyle = FoundDocumentStyle;
		foundDocumentName = FoundDocumentName;
		sourceSentences = SourceSentences;
		foundSentences = FoundSentences;
		certainty = Certainty;
	}
};

// used to return the list of certainties that the document is a particular style.
struct StyleCertaintyItem
{
	int StyleID;
	string StyleName;
	double certainty;
	
	void print()
	{
	    std::cout << std::fixed << std::setprecision(2) << StyleID << " - " << StyleName << " : " << certainty << std::endl;
	}
};


// the final product that is returned from compare
struct CompareResult
{
	// The percentage of the document that is completely compared.  100 means the document is finished and the other information should be filled.
	int percentComplete;
	// a list of certainties for each style ranked by highest probability to lowest
	vector<StyleCertaintyItem> documentCertainties;
	// a list of the sentence that look suspicious throughout the document
	vector<SentenceRanking> sentenceRankings;
	
	void print()
	{
	    std::cout << "************************SentenceRanking***Begin Print ************************************" << std::endl;
	    std::cout << "Percent Complete: " << percentComplete << "%" << std::endl;
	    for (int i = 0; i < sentenceRankings.size(); i++)
	    {
		std::cout << " ************************  Orignial Sentence  *************************" << std::endl;
		sentenceRankings[i].sourceSentences.print();
		std::cout << " ************************  Copied From "<< sentenceRankings[i].foundDocumentName << "****************************" << std::endl;
		std::cout << "  " << sentenceRankings[i].foundDocumentStyle << " : " << sentenceRankings[i].foundDocumentName << std::endl;
		sentenceRankings[i].foundSentences.print();
	    }
	    for (int i =0;i<documentCertainties.size();i++)
	    {
		documentCertainties[i].print();
	    }
	    std::cout << " *************************** End Print ********************************" << std::endl;
	    
	}
	/*
	CompareResult copy()
	{
	    CompareResult cr;
	    cr.percentComplete = percentComplete;
	    
	    for (int i = 0; i < sentenceRankings.size(); i++)
	    {
		
	        SentenceBlob sourceBlob;
		sourceBlob.PrevPrevSentenceStr = sentenceRankings[i].sourceSentences.PrevPrevSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		sourceBlob.PrevSentenceStr = sentenceRankings[i].sourceSentences.PrevSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		sourceBlob.SentenceStr = sentenceRankings[i].sourceSentences.SentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		sourceBlob.NextSentenceStr = sentenceRankings[i].sourceSentences.NextSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		sourceBlob.NextNextSentenceStr = sentenceRankings[i].sourceSentences.NextNextSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		
	        SentenceBlob databaseBlob;
		databaseBlob.PrevPrevSentenceStr = sentenceRankings[i].foundSentences.PrevPrevSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		databaseBlob.PrevSentenceStr = sentenceRankings[i].foundSentences.PrevSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		databaseBlob.SentenceStr = sentenceRankings[i].foundSentences.SentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		databaseBlob.NextSentenceStr = sentenceRankings[i].foundSentences.NextSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		databaseBlob.NextNextSentenceStr = sentenceRankings[i].foundSentences.NextNextSentenceStr;  // The sentence id on the server that most closely matches the source sentence.
		
		cr.sentenceRankings.push_back(SentenceRanking(sentenceRankings[i].foundDocumentStyle, sentenceRankings[i].foundDocumentName, sourceBlob, databaseBlob, sentenceRankings[i].certainty));
	    }
	    for (int i =0;i<documentCertainties.size();i++)
	    {
		StyleCertaintyItem ci;
		ci.certainty = documentCertainties[i].certainty;
		ci.StyleName = documentCertainties[i].StyleName;
		ci.StyleID = documentCertainties[i].StyleID;
		cr.documentCertainties.push_back(ci);
	    }
	    return cr;
	    
	}*/
};


struct CreateResult
{
  std::string newDocument;
  int percentComplete;
};


//struct StyleScore
//{
//	int StyleID;
//	string StyleName;
//	float score;
//};

//struct compareResult
//{
//	vector<StyleScore> styleScores;
//	vector<SentenceRanking> sentenceRankings;
//	MetaData sourceDocument;
//};



struct StyleCounts
{
	int StyleID;
	string Author;
	int Count;

	StyleCounts()
	{
		StyleID = -1;
		Author = "";
		Count = 0;
	}

	StyleCounts(int in_styleID, string in_author, int in_count)
	{
		StyleID = in_styleID;
		Author = in_author;
		Count = in_count;
	}
};

struct StyleCounter
{
	int SentenceCount;
	int TotalCount;
	double SentencePercent;
	double TotalDocPercent;
	StyleCounter(int in_count) 
	{
		SentenceCount = in_count; 
		TotalCount = in_count;
	}

};


