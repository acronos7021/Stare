#include "Database.h"
StyleDatabase::StyleDatabase(string dbName)
{
	// Convert the dbName string to a format sqlite3_open needs
	const char * c = dbName.c_str();

	if (sqlite3_open(c, &db) == SQLITE_OK)
	{

	}
	else {
		throw exception("SQLite database failed to open");
	}
}


StyleDatabase::~StyleDatabase(void)
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
	insert("DELETE FROM Sentences");
	insert("DELETE FROM Styles");
	insert("DELETE FROM Tokens");
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
					retAns = atoi(s.c_str());
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

		return retAns;
	}
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
	string str = "select StyleID from Styles where author = '" + author + "';";
	char *query2 = &str[0];
	int retAns = -1;  // if the style isn't found, return this.

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

int StyleDatabase::getStyleID(int docID)
{
	string str = "select StyleID from Documents where DocumentID = " + std::to_string(docID) + ";";
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
	//cout << &str[0] << endl;

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
		int styleID = getStyleID(documentID);
		if (styleID == -1)
		{
			insertAuthor(Author);
			insertDocumentIntoDB(getStyleID(documentID), Title, PublishDate);
		}
		else {
			insertDocumentIntoDB(styleID, Title, PublishDate);
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
	int count = 0;
	insertIntoSentences(DocumentID);
	int sentID = getSentenceID(DocumentID);
	int styleID = getStyleID(DocumentID);
	vector<int> wordIDs;

	while (count < words.size())
	{
		if (doesWordExist(words[count]) == true)
		{
			// Just get the word id if its already in the db
			wordIDs[count] = getWordID(words[count]);
		}
		else {
			addWord(words[count]); // add the word to the database
			wordIDs[count] = getWordID(words[count]);
		}
		count = count + 1;
	}

	count = 0; // reset counter
	while (count < wordIDs.size())
	{
		if ((count - 1) < 0)
		{
			if ((count + 1) > wordIDs.size())
			{
				addHMMTokenPath(sentID, styleID, wordIDs[count],-1,-1);
			}
			else {
				// add current word and next token
				addHMMTokenPath(sentID, styleID, wordIDs[count], wordIDs[count + 1], -1);
			}
		}
		else {
			if (count + 1 > wordIDs.size())
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

	return sentID;  
}



// Returns the string of the selected sentence. 
string StyleDatabase::getSentence(int sentenceID)
{
	string str = "SELECT Tokens.Word FROM HMMtokenPaths JOIN Tokens ON Tokens.TokenID = HMMtokenPaths.CurrentToken WHERE HMMtokenPaths.SentenceID = sentenceID ORDER BY HMMtokenPaths.TokenPathID;";
	char* s;
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
				}
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				break;
			}
		}
	}

	return s;
}

// returns the number of instances (the count) in all documents of that series of 3 words.
//	Returns a list of the SentenceID’s for every occurance of that string. 
int StyleDatabase::getWordGroupCount(string prevWord, string currWord, string nextWord)
{
	string str = "SELECT COUNT(CurrentToken) FROM HMMtokenPaths	WHERE CurrentToken = currWord AND NextToken = nextWord AND PreviousToken = prevWord;";
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
	string str = "SELECT COUNT(CurrentToken) FROM HMMtokenPaths WHERE StyleID = styleID AND CurrentToken = currWord AND NextToken = nextWord AND PreviousToken = prevWord";
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
	string str = "SELECT CurrentToken FROM HMMtokenPaths WHERE StyleID = styleID AND CurrentToken = currWord AND NextToken = nextWord AND PreviousToken = prevWord;";
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

// Test function
vector<string> StyleDatabase::testFunc()
{
	vector<string> test;
	test[0] = "this";
	test[1] = "is";
	test[2] = "a";
	test[3] = "test";
	return test;
}