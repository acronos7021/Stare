#include <cstdio>
#include <unordered_map>
#include <chrono>
#include <thread>
#include "Database.h"
#include "WordPairCountDatabase.h"

StyleDatabase::StyleDatabase()
{
	isOpen = false;

	lastToken = 0;
	lastAllocatedTokenID = 0;
	currentAllocatedTokenID = 0;
}


StyleDatabase::~StyleDatabase(void)
{
	close();// sqlite3_close(db); // Try to close the database
}

void StyleDatabase::open()
{
	// Convert the dbName string to a format sqlite3_open needs
	//const char * c = dbName.c_str();

	if (sqlite3_open("AIsql.db3", &db) == SQLITE_OK)
	{
		isOpen = true;
	}
	else {
		isOpen = false;
		throw exception("SQLite database failed to open");
	}

	lastToken = 0;
	lastAllocatedTokenID = 0;
	currentAllocatedTokenID = 0;
	LoadTokenMap();
}

void StyleDatabase::close()
{
	sqlite3_close(db); // Try to close the database
}


void StyleDatabase::insert(string q)
{
	stringstream b;
	b << q;
	string s = b.str();
	char *str = &s[0];
	int result;
	char *query = str;
	{
		int prepCode = sqlite3_prepare(db, query, -1, &statement, 0);
		if (prepCode == SQLITE_OK)
		{
			int res = sqlite3_step(statement);
			result = res;
			sqlite3_finalize(statement);
		}
		else
		{
			cout << "Error executing the following SQL command" << endl;
			cout << q << endl;
			cout << "Error Code:" << prepCode;
		}
	}
}

void StyleDatabase::clearDatabase()
{



	insert("DELETE FROM Documents");
	insert("DELETE FROM HMMTokenPaths");
//	insert("DELETE FROM Sentences");
	insert("DELETE FROM Styles");
	insert("DELETE FROM Variables");
	insert("INSERT INTO Variables (LastSentenceID,LastTokenID) VALUES (0,2)");
	insert("DELETE FROM Tokens");
	insert("INSERT INTO Tokens (TokenID,Word) VALUES (1,CHAR(13))");
	insert("INSERT INTO Tokens (TokenID,Word) VALUES (2,CHAR(9))");
	LoadTokenMap();
}





void StyleDatabase::addHMMTokenPath(int SentID, int StyleID, int CurToken, int NextToken, int PrevToken)
{
	insert("INSERT INTO HMMTokenPaths (SentenceID,StyleID,CurrentToken,NextToken,PreviousToken) VALUES(" + std::to_string(SentID) + "," + std::to_string(StyleID) + "," + std::to_string(CurToken) + "," + std::to_string(NextToken) + ","+std::to_string(PrevToken)+");");
}

/* gets a Word ID */
int StyleDatabase::getWordID(string word)
{
	string str = "select TokenID from Tokens where word = '" + word + "';";
	char *query2 = &str[0];
	int retAns = 0;

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					string s = (char*)sqlite3_column_text(statement, i);
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

/* Adds a Document into the Sentences DB */
void StyleDatabase::insertIntoSentences(int docid)
{
	string str = "INSERT INTO Sentences (DocumentID) VALUES(" + std::to_string(docid) + ");";
	insert(str);
}

/* Adds a word into the Database */
void StyleDatabase::addWord(string word)
{
	string str = "INSERT INTO Tokens (Word) VALUES('" + word + "');";
	insert(str);
}

/* Checks to see if a Word exists; true if it does -- false if it doesn't */
bool StyleDatabase::doesWordExist(string word)
{
	string str = "select TokenID from Tokens where word = '" + word + "';";
	char *query2 = &str[0];
	bool retAns = false;

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					string s = (char*)sqlite3_column_text(statement, i);
					retAns =  atoi(s.c_str())>0;
				}
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}

		/* retAns is set to false if the word isn't found; otherwise it's set to true */
		if (retAns == NULL)
		{
			retAns = false;
		}
		else {
			retAns = true;
		}
	}
	return retAns;
}

/* Inserts an Author into the Database */
void StyleDatabase::insertAuthor(string author)
{
	string str = "INSERT INTO Styles (Author) VALUES('" + author + "');";
	insert(str);
}

/* Retrieve the Author Style */
int StyleDatabase::retrieveAuthorStyleID(string author)
{
	sqlite3_stmt* stmt;
	string str = "select StyleID from Styles where author = '" + author + "';";
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
					string s = (char*)sqlite3_column_text(stmt, i);
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

int StyleDatabase::getStyleID(int docID)
{
	sqlite3_stmt* stmt;
	string str = "select StyleID from Documents where DocumentID = " + std::to_string(docID) + ";";
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
					string s = (char*)sqlite3_column_text(stmt, i);
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

int StyleDatabase::getSentenceID(int docid)
{
	string str = "select SentenceID from Sentences where DocumentID = " + std::to_string(docid) + ";";
	char *query2 = &str[0];
	int retAns = 0;
	 
	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					string s = (char*)sqlite3_column_text(statement, i);
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


int StyleDatabase::getDocumentID(string Author, string title)
{
	int styleID = retrieveAuthorStyleID(Author);
	if (styleID == -1) return -1;
	else return getDocumentID(styleID, title);
}

int StyleDatabase::getDocumentID(int StyleID, string title)
{
	string str = "select DocumentID from Documents where title = '" + title + "' AND StyleID = '" + std::to_string(StyleID) + "';";
	char *query2 = &str[0];
	int retAns = -1; // if the document isn't found, return this.

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					string s = (char*)sqlite3_column_text(statement, i);
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

/* Use this if you want to check to see if something exists in the database */
int StyleDatabase::retrieve(string table, string data, string searchType, string searchData)
{
	string str = "select " + data + " from " + table + " where " + searchType + " = '" + searchData + "';";
	char *query2 = &str[0];
	int retAns = 0;

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					string s = (char*)sqlite3_column_text(statement, i);
					if (s.empty() == true)
					{
						retAns = 0;
					}
					else {
						retAns = 1;
					}
				}
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
	}
	else {
		cout << "Possible SQL Error" << endl;
	}
	return retAns;
}

void StyleDatabase::insertDocumentIntoDB(int styleID, string title, string publishDate)
{
	string str = "INSERT INTO Documents (StyleID,Title,PublishDate) VALUES('" + std::to_string(styleID) + "','" + title + "','" + publishDate + "');";
	insert(str);
}


//Use this to create a new Document
//  First check if the document already exists in the database.
// if it does, just return that ID.
//  Second, check if a Style (author) doesn’t exist 
// if not, add it to the Style table then create new document.  
// returns the DocumentID of the new Document.

int StyleDatabase::insertDocument(string Author, string Title, string PublishDate)
{
	int documentID = getDocumentID(Author, Title);
	if (documentID == -1)
	{
		int styleID = retrieveAuthorStyleID(Author);
		if (styleID == -1)
		{
			insertAuthor(Author);
			insertDocumentIntoDB(retrieveAuthorStyleID(Author), Title, PublishDate);
			documentID = getDocumentID(Author, Title);
		}
		else {
			insertDocumentIntoDB(styleID, Title, PublishDate);
			documentID = getDocumentID(Author, Title);
		}
	}
	else {
		// Just Return DocumentID
	}

	return documentID;
}

// Used to add a sentence
// This would:
//	1) convert the vector of words to a vector of integer tokens
//	2) add any new words to the Token table(if needed)
//	3) add these tokens to the Sentence table creating a new row.
//	4) add these tokens to the HMMtokenPath table (along with the StyleID (derived from DocumentID) 
//     and SentenceID (which is remembered from #3).
//     Each word would be a row in the HMMtokenPath table along with the word before it and the word after it.
//	5) Returns the new sentenceID. 
// I believe this will be best handled on the server side using a stored procedure
int StyleDatabase::insertSentence(int DocumentID, vector<string> words)
{

	//char* errorMessage;
	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
	int count = 0;
	insertIntoSentences(DocumentID);
	int sentID = getSentenceID(DocumentID);
	int styleID = getStyleID(DocumentID);
	vector<int> wordIDs;


	while (count < (int) words.size())
	{
		//bool dWE = doesWordExist(words[count]);
		int tokenID = getWordID(words[count]);
		if (tokenID > 0)
		{
			// Just get the word id if its already in the db
			wordIDs.push_back(tokenID);
		}
		else {
			addWord(words[count]); // add the word to the database
			wordIDs.push_back(getWordID(words[count]));
		}
		count = count + 1;
	}


	count = 0; // reset counter
	while (count < (int) wordIDs.size())
	{
		if ((count - 1) < 0)
		{
			if ((count + 1) > (int)wordIDs.size())
			{
				addHMMTokenPath(sentID, styleID, wordIDs[count],-1,-1);
			}
			else {
				// add current word and next token
				addHMMTokenPath(sentID, styleID, wordIDs[count], wordIDs[count + 1], -1);
			}
		}
		else {
			if (count + 1 >= (int) wordIDs.size())
			{
				// just add prev token and cur token
				addHMMTokenPath(sentID, styleID, wordIDs[count], -1, wordIDs[count - 1]);
			}
			else {
				// add all three
				addHMMTokenPath(sentID, styleID, wordIDs[count], wordIDs[count + 1], wordIDs[count - 1]);
			}
		}
		count = count + 1;
	}
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
	return sentID;  
}



// Returns the string of the selected sentence. 
string StyleDatabase::getSentence(int sentenceID)
{
	string str = "SELECT Tokens.Word FROM HMMtokenPaths JOIN Tokens ON Tokens.TokenID = HMMtokenPaths.CurrentToken WHERE HMMtokenPaths.SentenceID = "+ std::to_string(sentenceID) +" ORDER BY HMMtokenPaths.TokenPathID;";
	char* s;
	char *query2 = &str[0];
	stringstream tmp;

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					 s = (char*)sqlite3_column_text(statement, i);
					 tmp << s << " ";
				}
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
	}

	return tmp.str();
}

// returns the number of instances (the count) in all documents of that series of 3 words.
//	Returns a list of the SentenceID’s for every occurance of that string. 
int StyleDatabase::getWordGroupCount(string prevWord, string currWord, string nextWord)
{
	string str = "SELECT COUNT(CurrentToken) FROM HMMtokenPaths	WHERE CurrentToken = '"+currWord+"' AND NextToken = '"+nextWord+"' AND PreviousToken = '"+prevWord+"';";
	string s;
	char *query2 = &str[0];
	int retAns = 0;

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					s = (char*)sqlite3_column_text(statement, i);
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

//	same as above but restricts the count to a specific style.
int StyleDatabase::getWordGroupCountByStyle(int StyleID, string prevWord, string currWord, string nextWord)
{
	//SELECT COUNT(CurrentToken) FROM HMMtokenPaths WHERE StyleID = styleID AND CurrentToken = currWord AND NextToken = nextWord AND PreviousToken = prevWord;
	string str = "SELECT COUNT(CurrentToken) FROM HMMtokenPaths WHERE StyleID = "+ std::to_string(StyleID) +" AND CurrentToken = '"+ currWord +"' AND NextToken = '"+nextWord+"' AND PreviousToken = '"+nextWord+"'";
	string s;
	char *query2 = &str[0];
	int retAns = 0;

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					s = (char*)sqlite3_column_text(statement, i);
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

//Returns a list of all of the SentenceIDs where that combination of words exists in all documents of the selected style. 
vector<int> StyleDatabase::getWordGroupListByStyle(int StyleID, string prevWord, string currWord, string nextWord)
{
	string str = "SELECT CurrentToken FROM HMMtokenPaths WHERE StyleID = " + std::to_string(StyleID) + " AND CurrentToken = '"+currWord+"' AND NextToken = '"+nextWord+"' AND PreviousToken = '"+prevWord+"';";
	char *query2 = &str[0];
	int retAns = 0;


	vector<int> tokens;

	if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
	{
		int coltotal = sqlite3_column_count(statement);
		int res = 0;
		int count = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < coltotal; i++)
				{
					string s = (char*)sqlite3_column_text(statement, i);
					tokens[count] = atoi(s.c_str());
					count = count + 1;
				}
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
	}

	return tokens;
}

int StyleDatabase::incrementSentenceID(int byAmount)
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
			string updateStr = "UPDATE Variables SET LastSentenceID = LastSentenceID + " + byAmount;
			sqlite3_exec(db, updateStr.c_str(), NULL, NULL, NULL);
		}
		else
		{
			// the variable doesn't exist in the database, so create it and return 0;
			stringstream insertStr;
			insertStr << "INSERT INTO Variables (LastSentenceID) VALUES (" << byAmount << ")";
			sqlite3_exec(db, insertStr.str().c_str(), NULL, NULL, NULL);
			currentSentenceID = 0;
		}
	}
	else
	{
		cout << "Unable to execute 'SELECT LastSentenceID FROM Variables'" << endl;
		cout << "Error Code: " << prepareCode << endl;
	}
	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
	return currentSentenceID;
}




void StyleDatabase::LoadTokenMap()
{
	//char* errorMessage;
	sqlite3_stmt *select_statement;

	string selectTokenStr = "SELECT TokenID,Word FROM Tokens WHERE TokenID >= " + std::to_string(lastToken); // There is a bug here.  If another process gets tokens allocated and then adds them, they will not show up here.
	if (sqlite3_prepare(db, selectTokenStr.c_str(), -1, &select_statement, 0) == SQLITE_OK)
	{
		int checkRow;
		while (true)
		{
			checkRow = sqlite3_step(select_statement);
			if (checkRow == SQLITE_ROW)
			{
				int tokenID = sqlite3_column_int(select_statement, 0);
				string word = (char*)sqlite3_column_text(select_statement, 1);
				tokenMap.insert(std::pair<string, int>(word, tokenID));
				if (tokenID > lastToken) lastToken = tokenID;
			}
			else break;
		}
	}
}

int StyleDatabase::GetToken(string word)
{
	map<string,int>::iterator it = tokenMap.find(word);
	if (it == tokenMap.end())
	{
		// the token does not exist in the database
		// so add it to the temp list and insert it into the map
		int nextTokenID = GetNextTokenID();
		std::pair<string, int> tokenPair = std::pair<string, int>(word, nextTokenID);
		tokenMap.insert(tokenPair);
		tokenCache.push_back(tokenPair);
		if (nextTokenID > lastToken) lastToken = nextTokenID;
		return nextTokenID;
	}
	else
		return it->second;
}

void StyleDatabase::FlushTokenCache()
{
	if (tokenCache.size() > 0)
	{
		//char* errorMessage;
		sqlite3_stmt *stmt;

		sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

		// setup pre-prepared sql statements.
		char buffer[] = "INSERT INTO Tokens VALUES (?1, ?2)";

		sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);

		for (vector<pair<string, int>>::iterator it = tokenCache.begin(); it != tokenCache.end(); ++it)
		{
			// bind to prepared statement. 
			sqlite3_bind_int(stmt, 1, it->second);
			sqlite3_bind_text(stmt, 2, it->first.c_str(),it->first.size(),0);
			sqlite3_step(stmt);
			sqlite3_reset(stmt);
		}
		sqlite3_finalize(stmt);
		sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
		tokenCache.clear();
	}
}

int StyleDatabase::GetNextTokenID()
{
	// get tokenIDs in 100 allocations.  Only ask for a new allocation when it is needed.
	if (currentAllocatedTokenID == lastAllocatedTokenID)
	{
		// get new batch of tokens from the server
		//char* errorMessage;
		sqlite3_stmt *select_statement;

		sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
		// get the current value
		if (sqlite3_prepare(db, "SELECT LastTokenID FROM Variables", -1, &select_statement, 0) == SQLITE_OK)
		{
			int checkRow = sqlite3_step(select_statement);
			if (checkRow == SQLITE_ROW)
			{
				// The variable exists in the database, so read it and update it.
				// we know there is something here or it wouldn't have given SQLITE_ROW, so just read first item.
				currentAllocatedTokenID = sqlite3_column_int(select_statement, 0);
				lastAllocatedTokenID = currentAllocatedTokenID + 100;
				string updateStr = "UPDATE Variables SET LastTokenID = " + std::to_string(lastAllocatedTokenID);
				sqlite3_exec(db, updateStr.c_str(), NULL, NULL, NULL);
			}
			else
			{
				// the variable doesn't exist in the database, so create it and return 0;
				//stringstream insertStr;
				//insertStr << "INSERT INTO Variables (SentenceID) VALUES (100)";
				sqlite3_exec(db, "INSERT INTO Variables (SentenceID) VALUES (100)", NULL, NULL, NULL);
				currentAllocatedTokenID = 0;
				lastAllocatedTokenID = 100;
			}
		}
		sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
	}
	return ++currentAllocatedTokenID;
}

vector<StyleCounts> StyleDatabase::getTotalWordCountPerStyle()
{
//	char* errorMessage;
	sqlite3_stmt *select_statement;
	vector<StyleCounts> output;

	int StyleID, wordCount;
	string Author;

	char* selectStr =
		"SELECT HMMtokenPaths.StyleID, Styles.Author, COUNT(HMMtokenPaths.CurrentToken) FROM HMMtokenPaths JOIN Styles ON Styles.StyleID = HMMtokenPaths.StyleID GROUP BY HMMtokenPaths.StyleID";

	if (sqlite3_prepare(db, selectStr, -1, &select_statement, 0) == SQLITE_OK)
	{
		while (true)
		{
			int checkRow = sqlite3_step(select_statement);
			if (checkRow == SQLITE_ROW)
			{
				// The variable exists in the database, so read it and update it.
				// we know there is something here or it wouldn't have given SQLITE_ROW, so just read first item.
				StyleID = sqlite3_column_int(select_statement, 0);
				Author = (char*)sqlite3_column_text(select_statement, 1);
				wordCount = sqlite3_column_int(select_statement, 2);
				output.push_back(StyleCounts(StyleID, Author, wordCount));
			}
			else break;
		}
	}
	return output;
}

vector<StyleCounts> StyleDatabase::getPathWordCountPerStyle(int currToken, int nextToken)
{
//	char* errorMessage;
	sqlite3_stmt *select_statement;
	vector<StyleCounts> output;

	int StyleID, wordCount;
	string Author;

	char selectStr[500];
	sprintf_s(selectStr, 500,
	   "SELECT Styles.StyleID, Styles.Author, COUNT(HMMtokenPaths.CurrentToken) FROM HMMtokenPaths	JOIN Styles ON Styles.StyleID = HMMtokenPaths.StyleID WHERE CurrentToken = %d AND NextToken = %d GROUP BY HMMtokenPaths.StyleID",
	   currToken, nextToken);

	if (sqlite3_prepare(db, selectStr, -1, &select_statement, 0) == SQLITE_OK)
	{
		while (true)
		{
			int checkRow = sqlite3_step(select_statement);
			if (checkRow == SQLITE_ROW)
			{
				// The variable exists in the database, so read it and update it.
				// we know there is something here or it wouldn't have given SQLITE_ROW, so just read first item.
				StyleID = sqlite3_column_int(select_statement, 0);
				Author = (char*)sqlite3_column_text(select_statement, 1);
				wordCount = sqlite3_column_int(select_statement, 2);
				output.push_back(StyleCounts(StyleID, Author, wordCount));
			}
			else break;
		}
	}
	return output;
}


void StyleDatabase::CreateDatabase()
{
	char* errorMessage;
	sqlite3_stmt *select_statement;
	string proceedWithOperation;

	cout << "This operation will destroy any existing database." << endl << " If you want to prceed anyway, enter 'create' : ";
	cin >> proceedWithOperation;

	if (proceedWithOperation != "create")
	{
		cout << "Operation Canceled!";
		return;
	}

	// close the current database
	close();

	// delete the current database file
	int ret_code = std::remove("AIsql.db3");

	if (ret_code != 0)
	{
		//cout << "Unable to delete file 'AIsql.db3'" << endl << "error code: " << strerror << endl;
		perror("Unable to delete database");
		cout << endl << "Attempting to clean existing database instead" << endl;
		vector<string> tables;

		open();
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
			return;
		}
		for (unsigned int i = 0; i < tables.size();i++)
		{
			cout << "Dropping " << tables[i] << endl;
			char dropStr[500];
			sprintf_s(dropStr, 500, "DROP TABLE %s", tables[i].c_str());
			if (sqlite3_exec(db, dropStr, NULL, NULL, &errorMessage) != SQLITE_OK)
			{
				cout << errorMessage << endl;
				cout << "Operation Failed!" << endl;
				sqlite3_free(errorMessage);
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
	else
	{
		// open the clean database
		open();
	}

	// create the tables
	cout << "Creating Documents table" << endl;
	sqlite3_exec(db, "CREATE TABLE Documents ( \
		DocumentID  INTEGER PRIMARY KEY AUTOINCREMENT\
		NOT NULL,\
		StyleID     INTEGER NOT NULL,\
		Title       TEXT,\
		PublishDate TEXT)"
		, NULL, NULL, NULL);

	cout << "Creating HMMtokenPaths table" << endl;
	sqlite3_exec(db, "CREATE TABLE HMMtokenPaths ( \
		TokenPathID   INTEGER PRIMARY KEY AUTOINCREMENT\
		NOT NULL,\
		StyleID       INTEGER NOT NULL,\
		DocumentID    INTEGER NOT NULL,\
		SentenceID    INTEGER NOT NULL,\
		CurrentToken  INTEGER NOT NULL,\
		NextToken     INTEGER NOT NULL,\
		PreviousToken INTEGER NOT NULL)"
		, NULL, NULL, NULL);


	cout << "Creating Styles table" << endl;
	sqlite3_exec(db, "CREATE TABLE Styles ( \
		StyleID INTEGER NOT NULL\
		PRIMARY KEY AUTOINCREMENT,\
		Author  TEXT    NOT NULL)"
		, NULL, NULL, NULL);

	cout << "Creating Tokens table" << endl;
	sqlite3_exec(db, "CREATE TABLE Tokens ( \
		TokenID INTEGER NOT NULL\
		PRIMARY KEY AUTOINCREMENT,\
		Word    TEXT)"
		, NULL, NULL, NULL);

	cout << "Creating Variables table" << endl;
	sqlite3_exec(db, "CREATE TABLE Variables ( \
		LastSentenceID INT PRIMARY KEY,\
		LastTokenID    INT)"
		, NULL, NULL, NULL);

	cout << "Setup default values" << endl;
	// Load Initial Values
	insert("INSERT INTO Variables (LastSentenceID,LastTokenID) VALUES (0,2)");

	insert("INSERT INTO Tokens (TokenID,Word) VALUES (1,CHAR(13))");
	insert("INSERT INTO Tokens (TokenID,Word) VALUES (2,CHAR(9))");

	LoadTokenMap();
}





///***********************************************************************  Moved functions






void StyleDatabase::insertDocumentText(int DocumentID, std::vector <std::vector<int>> document)
{
	char* errorMessage;

	FlushTokenCache();


	// Preallocate all of the sentenceIDs that we will need.
	int StyleID = getStyleID(DocumentID);
	int numberOfSentences = document.size();
	int startSentenceID = incrementSentenceID(numberOfSentences); // returned from the server the first available SentenceID

	sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &errorMessage);

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
		}
	}

	char word_counts_buffer[] = "INSERT INTO WordCounts (CurrentToken,NextToken,Count) VALUES (?1, ?2, ?3)";// "StyleID,DocumentID,SentenceID,CurrentToken,NextToken,PreviousToken) VALUES (?1, ?2, ?3, ?4, ?5, ?6)";
	sqlite3_stmt* word_counts_stmt;
	int error2 = sqlite3_prepare_v2(db, word_counts_buffer, strlen(word_counts_buffer), &word_counts_stmt, NULL);

	sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, &errorMessage);
	sqlite3_finalize(HMMtokenPaths_stmt);
}


