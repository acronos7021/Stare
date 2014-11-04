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

struct sentenceRanking
{
	int sentenceID;		  // The selected sentence in the document that is being compared.
	int foundDocumentID;  // the document id on the server that matches the new source sentence.
	int foundDocumentName; // the name of the document on the server tha matches the new source sentence
	int foundSentenceID;  // The sentence id on the server that most closely matches the source sentence.
	double certainty;	  // The certainty that a certain sentence is the particular style listed.

	sentenceRanking(int SentenceID, int FoundDocumentID, int FoundSentenceID, double Certainty)
	{
		sentenceID = SentenceID;
		foundDocumentID = FoundDocumentID;
		foundSentenceID = FoundSentenceID;
		certainty = Certainty;
	}
};

struct compareResult
{
	vector<StyleScore> styleScores;
	vector<sentenceRanking> sentenceRankings;
	MetaData sourceDocument;
};

struct StyleScore
{
	int StyleID;
	string StyleName;
	float score;
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


