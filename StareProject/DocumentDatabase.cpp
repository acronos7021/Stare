#include <iostream>

#include "DocumentDatabase.h"
#include "WordPairCountDatabase.h"
#include "sqlite3.h"


DocumentDatabase::DocumentDatabase()
{
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
		throw std::exception("SQLite database failed to open");
	return db;
}

void DocumentDatabase::close(sqlite3 *db)
{
	sqlite3_close(db); // Try to close the database
}

/* Retrieve the Author Style */
int DocumentDatabase::getStyleID(sqlite3* db, std::string author)
{
	sqlite3_stmt* stmt;
	std::string str = "select StyleID from Styles where author = '" + author + "';";
	char *query2 = &str[0];
	int retAns = -1;  // if the style isn't found, return this.

	if (sqlite3_prepare(db, query2, -1, &stmt, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					std::string s = (char*)sqlite3_column_text(stmt, i);
					retAns = atoi(s.c_str());
				}
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

void DocumentDatabase::incrementWordStyleCounts(int StyleID, int count)
{
	// check if that style exists
	if ((int)TotalWordCountsByStyle.size() <= StyleID)
	{
		// Create a new item
		TotalWordCountsByStyle.resize(StyleID + 1);
		TotalWordCountsByStyle[StyleID] = StyleCounts(StyleID, "Error Creating TotalWordStyleCounts", 0);
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
	std::string str = "select DocumentID from Documents where title = '" + title + "' AND StyleID = '" + std::to_string(StyleID) + "';";
	char *query2 = &str[0];
	int retAns = -1; // if the document isn't found, return this.

	if (sqlite3_prepare(db, query2, -1, &stmt, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					std::string s = (char*)sqlite3_column_text(stmt, i);
					retAns = atoi(s.c_str());
				}
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
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
	createWordStyleCounts( StyleID, author);
	return StyleID;
}

int DocumentDatabase::insertDocument(sqlite3* db, int styleID, std::string title, std::string publishDate)
{
	std::string str = "INSERT INTO Documents (StyleID,Title,PublishDate) VALUES('" + std::to_string(styleID) + "','" + title + "','" + publishDate + "');";
	return insert(db,str);
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
	std::string str = "select StyleID from Documents where DocumentID = " + std::to_string(docID) + ";";
	char *query2 = &str[0];
	int retAns = 0;

	if (sqlite3_prepare(db, query2, -1, &stmt, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(stmt);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(stmt);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					std::string s = (char*)sqlite3_column_text(stmt, i);
					retAns = atoi(s.c_str());
				}
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
			std::string updateStr = "UPDATE Variables SET LastSentenceID = LastSentenceID + " + byAmount;
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
	}
	else
	{
		std::cout << "Unable to execute 'SELECT LastSentenceID FROM Variables'" << std::endl;
		std::cout << "Error Code: " << prepareCode << std::endl;
	}
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
	return currentSentenceID;
}

const vector<StyleCounts> DocumentDatabase::getTotalWordCountPerStyle()
{
	return TotalWordCountsByStyle;
}

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

void DocumentDatabase::insertDocumentText(int DocumentID, std::vector <std::vector<int>> document)
{
	//char* errorMessage;

	//FlushTokenCache();
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
		lastWordNum = document[sentenceNum].size();
		//TotalWordCountsByStyle[StyleID].Count += lastWordNum;
		incrementWordStyleCounts(StyleID, lastWordNum);
		for (wordNum = 0; wordNum < lastWordNum; wordNum++)
		{
			// Load all variables into the correct type to be inserted into the prepared statement.
			if (wordNum>0)
			{
				// middle or end of sentence
				prevWordToken = document[sentenceNum][wordNum - 1];
			}
			else
			{
				// Start of sentence
				prevWordToken = -1;
			}

			if (wordNum + 1 < lastWordNum)
			{
				// Start or middle of sentence
				nextWordToken = document[sentenceNum][wordNum + 1];
			}
			else
			{
				// end of sentence
				nextWordToken = -1;
			}

			currWordToken = document[sentenceNum][wordNum];
			//currWordSize = document[sentenceNum][wordNum].size();

			SentenceID = startSentenceID + sentenceNum;

			// bind to prepared statement. 
			sqlite3_bind_int(HMMtokenPaths_stmt, 1, StyleID);
			sqlite3_bind_int(HMMtokenPaths_stmt, 2, DocumentID);
			sqlite3_bind_int(HMMtokenPaths_stmt, 3, SentenceID);
			if (currWordToken != -1) sqlite3_bind_int(HMMtokenPaths_stmt, 4, currWordToken);
			else sqlite3_bind_null(HMMtokenPaths_stmt, 4);
			if (nextWordToken != -1) sqlite3_bind_int(HMMtokenPaths_stmt, 5, nextWordToken);
			else sqlite3_bind_null(HMMtokenPaths_stmt, 5);
			if (prevWordToken != -1) sqlite3_bind_int(HMMtokenPaths_stmt, 6, prevWordToken);
			else sqlite3_bind_null(HMMtokenPaths_stmt, 6);
			sqlite3_step(HMMtokenPaths_stmt);

			//sqlite3_clear_bindings(stmt);
			sqlite3_reset(HMMtokenPaths_stmt);
			wpd.AddTokenCount(currWordToken, nextWordToken, StyleID);
		}
	}

	char word_counts_buffer[] = "INSERT INTO WordCounts (CurrentToken,NextToken,Count) VALUES (?1, ?2, ?3)";// "StyleID,DocumentID,SentenceID,CurrentToken,NextToken,PreviousToken) VALUES (?1, ?2, ?3, ?4, ?5, ?6)";
	sqlite3_stmt* word_counts_stmt;
	int error2 = sqlite3_prepare_v2(db, word_counts_buffer, strlen(word_counts_buffer), &word_counts_stmt, NULL);

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
			char dropStr[500];
			sprintf_s(dropStr, 500, "DROP TABLE %s", tables[i].c_str());
			if (sqlite3_exec(db, dropStr, NULL, NULL, &errorMessage) != SQLITE_OK)
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
	insert(db,"INSERT INTO Variables (LastSentenceID,LastTokenID) VALUES (0,2)");

	insert(db, "INSERT INTO Tokens (TokenID,Word) VALUES (1,CHAR(13))");
	insert(db, "INSERT INTO Tokens (TokenID,Word) VALUES (2,CHAR(9))");
	close(db);
	//LoadTokenMap();
}