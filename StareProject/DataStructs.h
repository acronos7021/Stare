#pragma once
using namespace std;

#include <string>
#include <vector>

enum ActionType {Learn, Compare, Create};

struct MetaData
{
	ActionType action;
    string Author;
    string Title;
    string PublishDate;
    string DocumentText;
};

struct sentenceRanking
{
	int sentenceID;		  // The selected sentence in the document that is being compared.
	int foundDocumentID;  // the document id on the server that matches the new source sentence.
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

