#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <deque>
#include <stdexcept>
#include "WordPairCountDatabase.h"
#include "sqlite3.h"
#include "DataStructs.h"
#include "TokenDatabase.h"

struct Documents
{
	int DocumentID;
	std::string Author;
	std::string Title;
	std::string PublishDate;
	int startSentenceID;
	int endSentenceID;

	Documents()
	{
		// leave everything default
		DocumentID = 0;
	}

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

	Styles()
	{
		StyleID = 0;
		Author = "";
	}

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

	const std::string database_name = "AIsql.db3";

	static int databaseInstanceCount;
	// base class
	//sqlite3 *db;
	int insert(sqlite3* db, std::string q);
	sqlite3* openDB();
	void close(sqlite3 *db);
	bool isOpen;

	void initTables();
	
	// Stores the entire database of all sentences(tokenized) in all documents in the database
	// this is essentially the processed version of HMMtokenPath
	std::deque<std::vector<int>> TotalSentenceList;
	// Mirrors the "Documents" and "Styles" tables in sqlite
	vector<Documents> documentList;
	std::vector<Styles> StyleList;
	void addToDocumentList(int DocumentID, std::string Author, std::string Title, std::string PublishDate, int startSentenceID, int endSentenceID);
	//void ensureDocumentIDexists(DocumentID);

	void addToStyleList(int StyleID, std::string author); // adds to the local data struct.
	int insertStyle(sqlite3* db, std::string author); // adds to both the local data struct and sqlite database
	int getStyleID(std::string author);
	int getStyleID(sqlite3* db, std::string author);
	int getStyleID(sqlite3* db, int docID);
	int getStyleIDfromDocumentID(int DocID);
	std::string getAuthor(int StyleID);
	
	
	
	
	// core database used to calcula			wpd.AddCounts(currWordToken,nextwordToken,StyleID,sentenceID);te style probablilities and plagerism.
	WordPairCountDatabase wpd;
	
	// this is the total counts of all word pairs for each style.
	// it differes from the one in WordPairDatabase because that isolates the count to only that pair.
	//std::vector<StyleCounts> TotalWordCountsByStyle;

	//Use this to create a new Document
	//  if a Style (author) doesn’t exist and add it to the Style table.  
	//returns the DocumentID of the new Document.
	//int getDocumentListIndex(int DocumentID);
	//int getStyleListIndex(int StyleID);
	
	int insertDocument(std::string Author, std::string Title, std::string publishDate);
	void insertDocumentText(int DocumentID, std::deque<std::vector<int>> document);
	
	
	
	int insertDocument(sqlite3* db, int styleID, std::string title, std::string publishDate);

	int getDocumentID(sqlite3* db, int styleID, std::string title);
	int getDocumentID(sqlite3* db, std::string Author, std::string title);
	int GetDocIDfromSentID(int sentID);

	//int getSentenceID(int docID);
	int incrementSentenceID(sqlite3* db, int byAmount);

	bool isWordToken(int token);
	int getNextWordToken(const std::vector<int> &sentence, int currWordIndex);
	bool getPrevAndNext(int sentenceNum, int wordNum, int &prevWordToken, int &nextWordToken, const std::deque<std::vector<int>> &document);
	//void incrementWordStyleCounts(int StyleID, int count);
	void incrementTokenAndStyleCounts(vector<int> sentence, int StyleID, int SentenceID);

	//void createWordStyleCounts(int StyleID, std::string author);
	//const vector<StyleCounts> getTotalWordCountPerStyle();
	//vector<StyleCounts> getPathWordCountPerStyle(int currToken, int nextToken);
	void CreateDatabase(bool confirmation);

};

