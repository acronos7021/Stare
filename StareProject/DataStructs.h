#pragma once
using namespace std;

#include <string>
#include <vector>
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
};


// the final product that is returned from compare
struct CompareResult
{
	// a list of certainties for each style ranked by highest probability to lowest
	vector<StyleCertaintyItem> documentCertainties;
	// a list of the sentence that look suspicious throughout the document
	vector<SentenceRanking> sentenceRankings;
};


struct StyleCounts
{
	int StyleID;
	string Author;
	int Count;

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


