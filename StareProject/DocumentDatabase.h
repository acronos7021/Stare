#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <deque>
#include "WordPairCountDatabase.h"
#include "sqlite3.h"
#include "DataStructs.h"

struct Documents
{
	int DocumentID;
	std::string Author;
	std::string Title;
	std::string PublishDate;
	int startSentenceID;
	int endSentenceID;

	Documents(int docID, std::string author, std::string title, std::string pubDate, int start, int end)
	{
		DocumentID = docID;
		Author = author;
		Title = title;
		PublishDate = pubDate;
		startSentenceID = start;
		endSentenceID = end;
	}

};

struct Styles
{
	int StyleID;
	std::string Author;

	Styles(int styleID, std::string author)
	{
		StyleID = styleID;
		Author = author;
	}
};



class DocumentDatabase
{
public:
	DocumentDatabase();
	~DocumentDatabase();


	// base class
	//sqlite3 *db;
	int insert(sqlite3* db, std::string q);
	sqlite3* openDB();
	void close(sqlite3 *db);
	bool isOpen;

	
	std::deque<std::vector<int>> TotalSentenceList;
	vector<Documents> documentList;
	std::vector<Styles> StyleList;

	// to stay
	WordPairCountDatabase wpd;
	std::vector<StyleCounts> TotalWordCountsByStyle;

	//Use this to create a new Document
	//  if a Style (author) doesn’t exist and add it to the Style table.  
	//returns the DocumentID of the new Document.
	int getDocumentIndex(int DocumentID);
	int insertDocument(sqlite3* db, int styleID, std::string title, std::string publishDate);

	int getDocumentID(sqlite3* db, int styleID, std::string title);
	int getDocumentID(sqlite3* db, std::string Author, std::string title);

	int insertStyle(sqlite3* db, std::string author);
	int getStyleID(sqlite3* db, std::string author);
	int getStyleID(sqlite3* db, int docID);
	//int getSentenceID(int docID);
	int incrementSentenceID(sqlite3* db, int byAmount);

	bool isWordToken(int token);
	bool getPrevAndNext(int sentenceNum, int wordNum, int &prevWordToken, int &nextWordToken, const std::deque<std::vector<int>> &document);
	void incrementWordStyleCounts(int StyleID, int count);
	void createWordStyleCounts(int StyleID, std::string author);

	int insertDocument(std::string Author, std::string Title, std::string publishDate);
	void insertDocumentText(int DocumentID, std::deque<std::vector<int>> document);
	const vector<StyleCounts> getTotalWordCountPerStyle();
	vector<StyleCounts> getPathWordCountPerStyle(int currToken, int nextToken);
	void CreateDatabase(bool confirmation);

};

