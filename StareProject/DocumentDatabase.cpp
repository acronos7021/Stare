#include <iostream>
#include <stdexcept>
#include <cstring>
#include <set>


#include "DocumentDatabase.h"
#include "WordPairCountDatabase.h"
#include "sqlite3.h"
#include "Tokenizer.h"

// initialize static variable
int DocumentDatabase::databaseInstanceCount = 0;


DocumentDatabase::DocumentDatabase()
{
	if (databaseInstanceCount > 0)
		throw std::runtime_error("There can be only one!.  Somewhere else in the code another DocumentDatabase exists.");
	databaseInstanceCount++;
	//initTables();
}


DocumentDatabase::~DocumentDatabase()
{
}

sqlite3* DocumentDatabase::openDB()
{

	// Convert the dbName string to a format sqlite3_open needs
	//const char * c = dbName.c_str();




	sqlite3 *db;
	if (sqlite3_open("AIsql.db3", &db) != SQLITE_OK)
		throw std::runtime_error("SQLite database failed to open");
	return db;
}

void DocumentDatabase::close(sqlite3 *db)
{
	sqlite3_close(db); // Try to close the database
}

void DocumentDatabase::addToStyleList(int StyleID, std::string author)
{
	// make sure there is enought space in the vector.
	if ((int)StyleList.size() <= StyleID)
	{
		Styles dummy;
		while ((int)StyleList.size() <= StyleID)
		{
			// Create a new item
			StyleList.push_back(dummy);
		}
	}
	StyleList[StyleID] = Styles(StyleID, author);
}

void DocumentDatabase::addToDocumentList(int DocumentID, std::string Author, std::string Title, std::string PublishDate, int startSentenceID, int endSentenceID)
{
	// make sure there is enought space in the vector.
	if ((int)documentList.size() <= DocumentID)
	{
		Documents dummy;
		// Create a new item
		while ((int)documentList.size() <= DocumentID)
			documentList.push_back(dummy);
	}
	Documents newDoc(DocumentID,Author, Title, PublishDate, startSentenceID, endSentenceID);
	documentList[DocumentID] = newDoc;
}

void DocumentDatabase::initTables()
{
	// check to see if the database exists
	ifstream fin(database_name.c_str());
	if (fin.fail())
	{
		// can't find the database so create it.
		CreateDatabase(false);
	}
	fin.close();

	std::cout << std::endl << "  Initializing Database  " << std::endl;
	sqlite3* db = openDB();
	sqlite3_stmt* stmt;
	std::string selectStr = "SELECT StyleID,Author FROM Styles";
	if (sqlite3_prepare(db, selectStr.c_str(), -1, &stmt, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt);
			if (res == SQLITE_ROW)
			{
				int ID = sqlite3_column_int(stmt, 0);
				std::string aut = (char*)sqlite3_column_text(stmt, 1);
				addToStyleList(ID, aut);
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
		sqlite3_finalize(stmt);
	}
	sqlite3_stmt* stmt2;
	std::string selectStr2 = "SELECT StyleID,DocumentID,Title,PublishDate FROM Documents";
	if (sqlite3_prepare(db, selectStr2.c_str(), -1, &stmt2, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt2);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt2);
			if (res == SQLITE_ROW)
			{
				int newStyleID = sqlite3_column_int(stmt2, 0);
				int newDocID = sqlite3_column_int(stmt2, 1);
				std::string newTitle = (char*)sqlite3_column_text(stmt2, 2);
				std::string newPubDate = (char*)sqlite3_column_text(stmt2, 3);
				std::string author = StyleList[newStyleID].Author;
				addToDocumentList(newDocID, author, newTitle, newPubDate, 0, 0);
				//Documents newDoc(newDocID, author, newTitle, newPubDate, 0, 0);
				//documentList.push_back(newDoc);
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
		sqlite3_finalize(stmt2);
	}

	// keeps a status bar going
	int statUpdate = 0;

	sqlite3_stmt* stmt3;
	// ORDER BY DocumentID allows to find the start and end of a document
	// ORDER BY sentenceID ensures the sentences are in the correct order
	// ORDER BY TokenPathID ensures the words in a sentence are in the correct order
	std::string selectStr3 = "SELECT StyleID,DocumentID,SentenceID,CurrentToken FROM HMMtokenPaths ORDER BY DocumentID,SentenceID,TokenPathID";
	if (sqlite3_prepare(db, selectStr3.c_str(), -1, &stmt3, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt3);
		int res = 0;
		int currDocID = -1;
		int currSentID = -1;
		int sentID = -1;
		int lastWordToken = -1;
		vector<int> sentenceTokenIDs;
		while (1)
		{
			res = sqlite3_step(stmt3);
			if (res == SQLITE_ROW)
			{
//				StyleList.push_back(Styles(sqlite3_column_int(stmt3, 0), (char*)sqlite3_column_text(stmt3, 1)));
				int styleID = sqlite3_column_int(stmt3, 0);
				int docID = sqlite3_column_int(stmt3, 1);
				//int docIndex = getDocumentListIndex(docID);
				int lastSentID = sentID;
				int lastDocID = docID;
				sentID = sqlite3_column_int(stmt3, 2);
				if (docID != currDocID)
				{
				    documentList[docID].startSentenceID = sentID;
				    std::cout << std::endl << "Loading: " << documentList[docID].Author << ":" << documentList[docID].Title << std::endl;	
				    currDocID = docID;
//				  if (currDocID != -1)
//					{
//						// otherwise it is the first document so there is no previous document to end
//						if (docID >= 0) // check if the documentID is invalid
//							documentList[docID].endSentenceID = lastSentID; // use the sentID from last time.
//					}
//					// Starting a new document
//					documentList[docID].startSentenceID = sentID;
//					currDocID = docID;


				}
				if ((sentID != currSentID) && (currSentID != -1)) 
				{ 
					if (sentID < currSentID)
					{
						std::cout << "It Happened ";
					}
					// starting a new sentence 
					vector<int> dummy;
					if (currSentID == (int)TotalSentenceList.size())
					{
						// we are at the last item in the queue, so pushback should work
						TotalSentenceList.push_back(sentenceTokenIDs);
					}
					else if (currSentID < (int)TotalSentenceList.size())
					{
						// we are in the middle of the queue, so insert directly.
						TotalSentenceList[currSentID] = sentenceTokenIDs;
					}
					else
					{
						// we are outside the queue so expand the queue until it is large enough before pushback.
						while (currSentID >(int)TotalSentenceList.size())
							TotalSentenceList.push_back(dummy);
						TotalSentenceList.push_back(sentenceTokenIDs);
					}
					incrementTokenAndStyleCounts(sentenceTokenIDs, styleID,currSentID);
					currSentID = sentID;
					documentList[currDocID].endSentenceID = currSentID;
					sentenceTokenIDs.clear();
				}
				currSentID = sentID;
				int currToken = sqlite3_column_int(stmt3, 3);
				sentenceTokenIDs.push_back(currToken);
				///wpd.AddTokenCount(lastWordToken, currToken, styleID);
					// if sentences are out of order, the new id's need to be made

			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
			++statUpdate;
			if (statUpdate > 1000)
			{
				statUpdate = 0;
				std::cout << ".";
			}
		}

		sqlite3_finalize(stmt3);
	}
	close(db);
	std::cout << "." << std::endl;
}


void DocumentDatabase::incrementTokenAndStyleCounts( vector<int> sentence, int StyleID, int sentenceID)
{
    //incrementWordStyleCounts(StyleID, sentence.size());
    int currWordToken = -1;
    int nextWordToken = -1;


    for (unsigned int i = 0; i < sentence.size(); i++)
    {
	    if (isWordToken(sentence[i]))
	    {
		    currWordToken = nextWordToken;
		    nextWordToken = sentence[i];
		    //wpd.AddTokenPairCount(currWordToken, nextWordToken, StyleID);
		    wpd.AddCounts(currWordToken,nextWordToken,StyleID,sentenceID);

	    }
    }
    if (nextWordToken != -1)
    {
	    // The above code processes one token behind, so process that last token.
	    currWordToken = nextWordToken;
	    nextWordToken = -1;
	    //wpd.AddTokenPairCount(currWordToken,nextWordToken,StyleID);
	    wpd.AddCounts(currWordToken,nextWordToken,StyleID,sentenceID);

    }

}



int DocumentDatabase::getStyleIDfromDocumentID(int DocID)
{
	if ((DocID < 0) || (DocID >= documentList.size()))
		return -1;
	for (int i = 0; i < StyleList.size(); ++i)
	{
		if (StyleList[i].Author == documentList[DocID].Author)
			return StyleList[i].StyleID;
	}
	return -1;
}

 
/* Retrieve the Author Style */

int DocumentDatabase::getStyleID(std::string author)
{
//      sqlite3* db = openDB(); 
//      getStyleID(db,author);
//      close(db);
      
      for (int i =0; i < StyleList.size(); ++i)
      {
	  if (StyleList[i].Author == author)
	      return i;
      }
      return 0;
}


int DocumentDatabase::getStyleID(sqlite3* db, std::string author)
{
	sqlite3_stmt* stmt;
	std::string selectStr = "select StyleID from Styles where author = '" + author + "';";

	int retAns = -1;  // if the style isn't found, return this.

	if (sqlite3_prepare(db, selectStr.c_str(), -1, &stmt, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt);
			if (res == SQLITE_ROW)
			{
				retAns = atoi((char*)sqlite3_column_text(stmt, 0));
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
		sqlite3_finalize(stmt);
	}
	
	return retAns;
}

/*
void DocumentDatabase::incrementWordStyleCounts(int StyleID, int count)
{
	// check if that style exists
	if ((int)TotalWordCountsByStyle.size() <= StyleID)
	{
		// Create a new item
		TotalWordCountsByStyle.resize(StyleID + 1);
		TotalWordCountsByStyle[StyleID] = StyleCounts(StyleID, StyleList[StyleID].Author, 0);
	}
	else
	{
		TotalWordCountsByStyle[StyleID].Count += count;
	}
}


void DocumentDatabase::createWordStyleCounts( int StyleID, std::string author)
{
	// check if that style exists
	if ((int)TotalWordCountsByStyle.size() <= StyleID)
	{
		// Create a new item
		TotalWordCountsByStyle.resize(StyleID + 1);
	}
	TotalWordCountsByStyle[StyleID] = StyleCounts(StyleID, author, 0);
}
*/

int DocumentDatabase::GetDocIDfromSentID(int sentID)
{
	if ((sentID >= 0) && (sentID < TotalSentenceList.size()))
	{
	      for (int i = 0; i < documentList.size(); ++i)
	      {
		      if ((sentID >= documentList[i].startSentenceID) && (sentID < documentList[i].endSentenceID))
			      return i;
	      }
	}
	
	std::cout << "Invalid document ID# " << sentID << ".  Printing the list of all Document ranges. " << std::endl;
	for (int i = 0; i < documentList.size(); ++i)
	{
	     std::cout << documentList[i].Title << "  " << documentList[i].startSentenceID << " : " << documentList[i].endSentenceID << std::endl;
	}
	
	return -1;
}


int DocumentDatabase::getDocumentID(sqlite3* db, std::string Author, std::string title)
{
	int styleID = getStyleID(db,Author);
	if (styleID == -1) return -1;
	else return getDocumentID(db,styleID, title);
}

int DocumentDatabase::getDocumentID(sqlite3* db, int StyleID, std::string title)
{
	//sqlite3* db = openDB();
	sqlite3_stmt* stmt;
	std::string selectStr = "select DocumentID from Documents where title = '" + title + "' AND StyleID = '" + std::to_string(StyleID) + "';";

	int retAns = -1; // if the document isn't found, return this.
	if (sqlite3_prepare(db, selectStr.c_str(), -1, &stmt, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt);
			if (res == SQLITE_ROW)
			{
				retAns = atoi((char*)sqlite3_column_text(stmt, 0));
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
		sqlite3_finalize(stmt);
	}
	return retAns;
}

int DocumentDatabase::insert(sqlite3* db, std::string q)
{
	char* errorMessage;
	sqlite3_exec(db, q.c_str(), NULL, NULL, &errorMessage);
	if (errorMessage)
	{
		std::cout << "Error executing the following SQL command" << std::endl;
		std::cout << q << std::endl;
		std::cout << "Error message:" << errorMessage;
		sqlite3_free(errorMessage);
	}
	int ret = (int)sqlite3_last_insert_rowid(db);
	return ret;
	//sqlite3_stmt* stmt;
	//std::stringstream b;
	//b << q;
	//std::string s = b.str();
	//char *str = &s[0];
	//int result;
	//char *query = str;
	//{
	//	int prepCode = sqlite3_prepare(db, query, -1, &stmt, 0);
	//	if (prepCode == SQLITE_OK)
	//	{
	//		int res = sqlite3_step(stmt);
	//		result = res;
	//		sqlite3_finalize(stmt);
	//	}
	//	else
	//	{
	//		std::cout << "Error executing the following SQL command" << std::endl;
	//		std::cout << q << std::endl;
	//		std::cout << "Error Code:" << prepCode;
	//	}
	//}
	
}

/* Inserts an Author into the Database */
int DocumentDatabase::insertStyle(sqlite3* db, std::string author)
{
	std::string str = "INSERT INTO Styles (Author) VALUES('" + author + "');";
	int StyleID = insert(db,str);
	//int StyleID = getStyleID(author);
	//createWordStyleCounts( StyleID, author);
	//StyleList.push_back(Styles(StyleID, author));
	addToStyleList(StyleID, author);
	return StyleID;
}

std::string DocumentDatabase::getAuthor(int StyleID)
{
	for (unsigned int i = 0; i < StyleList.size(); i++)
	{
		if (StyleList[i].StyleID == StyleID)
			return StyleList[i].Author;
	}
	return "";
}

int DocumentDatabase::insertDocument(sqlite3* db, int styleID, std::string title, std::string publishDate)
{
	std::string str = "INSERT INTO Documents (StyleID,Title,PublishDate) VALUES('" + std::to_string(styleID) + "','" + title + "','" + publishDate + "');";
	int DocID = insert(db, str);
	addToDocumentList(DocID, getAuthor(styleID), title, publishDate, 0, 0);
	return DocID;
}

int DocumentDatabase::insertDocument(std::string Author, std::string Title, std::string PublishDate)
{

	sqlite3* db = openDB();
	int documentID = getDocumentID(db,Author, Title);
	if (documentID == -1)
	{
		int styleID = getStyleID(db,Author);
		if (styleID == -1)
		{
			styleID = insertStyle(db,Author);
			documentID = insertDocument(db, styleID, Title, PublishDate);
			//documentID = getDocumentID(Author, Title);
		}
		else {
			documentID = insertDocument(db,styleID, Title, PublishDate);
			//documentID = getDocumentID(Author, Title);
		}
	}
	else {
		// Just Return DocumentID
	}
	close(db);
	return documentID;
}


int DocumentDatabase::getStyleID(sqlite3* db, int docID)
{
	sqlite3_stmt* stmt;
	std::string selectStr = "select StyleID from Documents where DocumentID = " + std::to_string(docID) + ";";
	int retAns = 0;

	if (sqlite3_prepare(db, selectStr.c_str(), -1, &stmt, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt);
			if (res == SQLITE_ROW)
			{
				retAns = atoi((char*)sqlite3_column_text(stmt, 0));
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
		sqlite3_finalize(stmt);
	}
	return retAns;
}

int DocumentDatabase::incrementSentenceID(sqlite3* db, int byAmount)
{
	//char* errorMessage;
	sqlite3_stmt *select_statement;
	int currentSentenceID;
	int prepareCode;
	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
	// get the current value
	if ((prepareCode = sqlite3_prepare(db, "SELECT LastSentenceID FROM Variables", -1, &select_statement, 0)) == SQLITE_OK)
	{
		int checkRow = sqlite3_step(select_statement);
		if (checkRow == SQLITE_ROW)
		{
			// The variable exists in the database, so read it and update it.
			// we know there is something here or it wouldn't have given SQLITE_OK, so just read first item.
			currentSentenceID = sqlite3_column_int(select_statement, 0);
			std::string updateStr = "UPDATE Variables SET LastSentenceID = LastSentenceID + " + std::to_string(byAmount);
			sqlite3_exec(db, updateStr.c_str(), NULL, NULL, NULL);
		}
		else
		{
			// the variable doesn't exist in the database, so create it and return 0;
			std::stringstream insertStr;
			insertStr << "INSERT INTO Variables (LastSentenceID) VALUES (" << byAmount << ")";
			sqlite3_exec(db, insertStr.str().c_str(), NULL, NULL, NULL);
			currentSentenceID = 0;
		}
		sqlite3_finalize(select_statement);
	}
	else
	{
		std::cout << "Unable to execute 'SELECT LastSentenceID FROM Variables'" << std::endl;
		std::cout << "Error Code: " << prepareCode << std::endl;
	}
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
	return currentSentenceID;
}

/*
const vector<StyleCounts> DocumentDatabase::getTotalWordCountPerStyle()
{
	return TotalWordCountsByStyle;
}
*/

/*
vector<StyleCounts> DocumentDatabase::getPathWordCountPerStyle(int currToken, int nextToken)
{
	vector<StyleCounts> retVect;
	WordPairCountStruct countVect = wpd.getTokenCount(currToken, nextToken);
	for (int i = 0; i < (int) countVect.StyleCounts.size(); i++)
	{
		retVect.push_back(StyleCounts(i, TotalWordCountsByStyle[i].Author,countVect.StyleCounts[i]));
	}
	return retVect;
}
*/

bool DocumentDatabase::isWordToken(int token)
{
	// Words are only made out of letters, so only letters are considered word tokens.  
	if ((token >= 65 && token <= 90) || (token >= 97 && token <= 122))
		return true;
	else if (token > 255)
		return true;
	else
		return false;
}


int DocumentDatabase::getNextWordToken(const std::vector<int> &sentence, int currWordIndex)
{
	// set default value if next token is not found
	//nextWordToken = -1;
	// search for next token
	int lastToken = sentence.size();
	int nextNum = currWordIndex + 1;
	while (nextNum < lastToken)
	{
		if (isWordToken(sentence[nextNum]))
		{
			return sentence[nextNum];
			//nextWordToken = sentence[nextNum];
			//break;
		}
		++nextNum;
	}
	return -1;
}

// returns true if the currWordToken is actually a WordToken
bool DocumentDatabase::getPrevAndNext(int sentenceNum, int wordNum, int &prevWordToken, int &nextWordToken, const std::deque<std::vector<int>> &document)
{
	int lastWordNum = document[sentenceNum].size();
	if (isWordToken(document[sentenceNum][wordNum]))
	{
		int prevNum, nextNum;
		if (isWordToken(document[sentenceNum][wordNum]))
		{
			// set default value if previous token is not found
			prevWordToken = -1;
			//search for previous token
			prevNum = wordNum;
			while (prevNum > 0)
			{
				--prevNum;
				if (isWordToken(document[sentenceNum][prevNum]))
				{
					prevWordToken = document[sentenceNum][prevNum];
					break;
				}
			}

			// set default value if next token is not found
			nextWordToken = -1;
			// search for next token
			nextNum = wordNum;
			while (nextNum < lastWordNum - 1)
			{
				++nextNum;
				if (isWordToken(document[sentenceNum][nextNum]))
				{
					nextWordToken = document[sentenceNum][nextNum];
					break;
				}
			}
		}
		return true;
	}
	else
	{
		prevWordToken = -1;
		nextWordToken = -1;
		return false;
	}
}

//int DocumentDatabase::getStyleListIndex(int StyleID)
//{
//	for (unsigned int i = 0; i < StyleList.size(); i++)
//	{
//		if (StyleList[i].StyleID == StyleID)
//			return i;
//	}
//	return -1;
//
//}

//int DocumentDatabase::getDocumentListIndex(int DocumentID)
//{
//	for (unsigned int i = 0; i < documentList.size(); i++)
//	{
//		if (documentList[i].DocumentID == DocumentID)
//			return i;
//	}
//	return -1;
//}

void DocumentDatabase::insertDocumentText(int DocumentID, std::deque<std::vector<int>> document)
{
	//char* errorMessage;

	// add all of the sentences to the TotalSentenceList
	//int documentIndex = getDocumentListIndex(DocumentID);
	if ((DocumentID < 0) || DocumentID >= documentList.size())
	     return;   //  There is an error.  Kill the insert.  ensureDocumentIDexists(DocumentID);
	if (DocumentID != -1)
	{
		documentList[DocumentID].startSentenceID = TotalSentenceList.size();
		documentList[DocumentID].endSentenceID = TotalSentenceList.size() + document.size();
	}
	TotalSentenceList.insert(TotalSentenceList.end(), document.begin(), document.end());

	sqlite3* db = openDB();
	// Preallocate all of the sentenceIDs that we will need.
	int StyleID = getStyleID(db,DocumentID);
	int numberOfSentences = document.size();
  	int startSentenceID = incrementSentenceID(db,numberOfSentences); // returned from the server the first available SentenceID

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

	// setup pre-prepared sql statements.
	char hmm_buffer[] = "INSERT INTO HMMtokenPaths (StyleID,DocumentID,SentenceID,CurrentToken,NextToken,PreviousToken) VALUES (?1, ?2, ?3, ?4, ?5, ?6)";
	sqlite3_stmt* HMMtokenPaths_stmt;
	int error = sqlite3_prepare_v2(db, hmm_buffer, strlen(hmm_buffer), &HMMtokenPaths_stmt, NULL);

	// define required variables just once.
	int sentenceNum, wordNum, lastWordNum;

	int currWordToken, nextWordToken, prevWordToken;
	int SentenceID;
	// insert all words in all sentences into HMMtokenPaths
	for (sentenceNum = 0; sentenceNum < numberOfSentences; sentenceNum++)
	{
	  	SentenceID = startSentenceID + sentenceNum;
		lastWordNum = document[sentenceNum].size();
			//TotalWordCountsByStyle[StyleID].Count += lastWordNum;
		incrementTokenAndStyleCounts(document[sentenceNum], StyleID,SentenceID);
			//incrementWordStyleCounts(StyleID, lastWordNum); 
		for (wordNum = 0; wordNum < lastWordNum; wordNum++)
		{
			// Load all variables into the correct type to be inserted into the prepared statement.
			currWordToken = document[sentenceNum][wordNum];
			getPrevAndNext(sentenceNum, wordNum, prevWordToken, nextWordToken, document);
			//if (getPrevAndNext(sentenceNum, wordNum, prevWordToken, nextWordToken, document))
			//{
			//	wpd.AddTokenCount(currWordToken, nextWordToken, StyleID);
			//}



			// bind to prepared statement. 
			sqlite3_bind_int(HMMtokenPaths_stmt, 1, StyleID);
			sqlite3_bind_int(HMMtokenPaths_stmt, 2, DocumentID);
			sqlite3_bind_int(HMMtokenPaths_stmt, 3, SentenceID);
			sqlite3_bind_int(HMMtokenPaths_stmt, 4, currWordToken);
			sqlite3_bind_int(HMMtokenPaths_stmt, 5, nextWordToken);
			sqlite3_bind_int(HMMtokenPaths_stmt, 6, prevWordToken);
			//if (currWordToken != -1) sqlite3_bind_int(HMMtokenPaths_stmt, 4, currWordToken);
			//else sqlite3_bind_null(HMMtokenPaths_stmt, 4);
			//if (nextWordToken != -1) sqlite3_bind_int(HMMtokenPaths_stmt, 5, nextWordToken);
			//else sqlite3_bind_null(HMMtokenPaths_stmt, 5);
			//if (prevWordToken != -1) sqlite3_bind_int(HMMtokenPaths_stmt, 6, prevWordToken);
			//else sqlite3_bind_null(HMMtokenPaths_stmt, 6);
			sqlite3_step(HMMtokenPaths_stmt);
			//int sentenceID = (int)sqlite3_last_insert_rowid(db);
			//wpd.AddCounts(currWordToken,nextWordToken,StyleID,sentenceID);
			//sqlite3_clear_bindings(stmt);
			sqlite3_reset(HMMtokenPaths_stmt);

		}
	}

	//char word_counts_buffer[] = "INSERT INTO WordCounts (CurrentToken,NextToken,Count) VALUES (?1, ?2, ?3)";// "StyleID,DocumentID,SentenceID,CurrentToken,NextToken,PreviousToken) VALUES (?1, ?2, ?3, ?4, ?5, ?6)";
	//sqlite3_stmt* word_counts_stmt;
	//int error2 = sqlite3_prepare_v2(db, word_counts_buffer, strlen(word_counts_buffer), &word_counts_stmt, NULL);

	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
	sqlite3_finalize(HMMtokenPaths_stmt);
	close(db);
}


void DocumentDatabase::CreateDatabase(bool confirmation)
{
	char* errorMessage;
	sqlite3_stmt *select_statement;
	string proceedWithOperation;

	if (confirmation)
	{
		cout << "This operation will destroy any existing database." << endl << " If you want to prceed anyway, enter 'create' : ";
		cin >> proceedWithOperation;

		if (proceedWithOperation != "create")
		{
			cout << "Operation Canceled!";
			return;
		}
	}


	// delete the current database file
	int ret_code = std::remove("AIsql.db3");
	
	sqlite3* db = openDB();
	if (ret_code != 0)
	{
		//cout << "Unable to delete file 'AIsql.db3'" << endl << "error code: " << strerror << endl;
		perror("Unable to delete database");
		cout << endl << "Attempting to clean existing database instead" << endl;
		vector<string> tables;


		if (sqlite3_prepare(db, "SELECT name FROM sqlite_master WHERE type = 'table'", -1, &select_statement, 0) == SQLITE_OK)
		{
			while (true)
			{
				int checkRow = sqlite3_step(select_statement);
				if (checkRow == SQLITE_ROW)
				{
					string table = (char*)sqlite3_column_text(select_statement, 0);
					if (table != "sqlite_sequence")
						tables.push_back(table);
				}
				else
					break;
			}
		}
		else
		{
			cout << "Unable to list tables." << endl;
			cout << "Operation Failed!" << endl;
			close(db);
			return;
		}
		for (unsigned int i = 0; i < tables.size(); i++)
		{
			cout << "Dropping " << tables[i] << endl;
			//char dropStr[500];
			//sprintf(dropStr, 500, "DROP TABLE %s", );
			if (sqlite3_exec(db, tables[i].c_str(), NULL, NULL, &errorMessage) != SQLITE_OK)
			{
				cout << errorMessage << endl;
				cout << "Operation Failed!" << endl;
				sqlite3_free(errorMessage);
				close(db);
				return;
			}
		}
		if (sqlite3_exec(db, "DELETE FROM sqlite_sequence", NULL, NULL, &errorMessage) != SQLITE_OK)
		{
			cout << "Unable to reset the AutoIncrement fields!" << endl;
			cout << "Proceeding anyway." << endl;
			sqlite3_free(errorMessage);
		}
	}


	// create the tables
	cout << "Creating Documents table" << endl;
	insert(db, "CREATE TABLE Documents ( \
			   	DocumentID  INTEGER PRIMARY KEY AUTOINCREMENT\
				NOT NULL,\
				StyleID     INTEGER NOT NULL,\
				Title       TEXT,\
				PublishDate TEXT)");
				

	cout << "Creating HMMtokenPaths table" << endl;
	insert(db, "CREATE TABLE HMMtokenPaths ( \
				TokenPathID   INTEGER PRIMARY KEY AUTOINCREMENT\
				NOT NULL,\
				StyleID       INTEGER NOT NULL,\
				DocumentID    INTEGER NOT NULL,\
				SentenceID    INTEGER NOT NULL,\
				CurrentToken  INTEGER NOT NULL,\
				NextToken     INTEGER NOT NULL,\
				PreviousToken INTEGER NOT NULL)");


	cout << "Creating Styles table" << endl;
	insert(db, "CREATE TABLE Styles ( \
				StyleID INTEGER NOT NULL\
				PRIMARY KEY AUTOINCREMENT,\
				Author  TEXT    NOT NULL)");

	cout << "Creating Tokens table" << endl;
	insert(db, "CREATE TABLE Tokens ( \
			    TokenID INTEGER NOT NULL\
			    PRIMARY KEY AUTOINCREMENT,\
			    Word    TEXT)");

	cout << "Creating Variables table" << endl;
	insert(db, "CREATE TABLE Variables ( \
			   	LastSentenceID INT PRIMARY KEY,\
				LastTokenID    INT)");

	cout << "Setup default values" << endl;
	// Load Initial Values
	insert(db,"INSERT INTO Variables (LastSentenceID,LastTokenID) VALUES (0,256)");
	insert(db, "BEGIN TRANSACTION");

	for (int i = 0; i < 256; i++)
	{
		stringstream ss;
		ss << "INSERT INTO Tokens (TokenID,Word) VALUES ("<<i<<",CHAR(" << i << "))";
		insert(db, ss.str().c_str());
		
	}
	insert(db, "COMMIT TRANSACTION");
	close(db);
	documentList.clear();
	StyleList.clear();
	TotalSentenceList.clear();
	wpd.clearAll();
}