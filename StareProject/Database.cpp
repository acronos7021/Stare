#include "Database.h"


StyleDatabase::StyleDatabase(void)
{
	if (sqlite3_open("aisql.db3", &db) == SQLITE_OK)
	{

		//char *query2 = "select * from Tokens where TokenID=1";
		//if (sqlite3_prepare(db, query2, -1, &statement, 0) == SQLITE_OK)
		//{
		//	int coltotal = sqlite3_column_count(statement);
		//	int res = 0;
		//	while (1)
		//	{
		//		res = sqlite3_step(statement);
		//		if (res == SQLITE_ROW)
		//		{
		//			for (int i = 0; i < coltotal; i++)
		//			{
		//				string s = (char*)sqlite3_column_text(statement, i);
		//				cout << s << endl;
		//			}
		//		}
		//		if (res == SQLITE_DONE || res == SQLITE_ERROR)
		//		{
		//			break;
		//		}
		//	}
		}


		//sqlite3_prepare(db, query,-1,&statement,0)
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
	strm << q;
	string s = strm.str();
	char *str = &s[0];
	int result;
	char *query = str;
	{
		if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK)
		{
			int res = sqlite3_step(statement);
			result = res;
			sqlite3_finalize(statement);
		}
	}
}

// Use this if you want to check to see if something exists in the database
int StyleDatabase::retrieve(string table, string data, string searchType, string searchData)
{
	//strm << "SELECT " << data << " from " << table << " LIMIT 1;";
	//char *query2 = "select " + data + " from Tokens where TokenID=1";
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

	return retAns;
}

//Use this to create a new Document
//  First check if the document already exists in the database.
// if it does, just return that ID.
//  Second, check if a Style (author) doesn’t exist 
// if not, add it to the Style table then create new document.  
// returns the DocumentID of the new Document.
// I believe this will be best handled on the server side using a stored procedure
int StyleDatabase::insertDocument(bool &alreadyExists, string Author, string Title, string publishDate)
{
	// CALL insertDocument(Author,Title,publishDate)
	//string query = "SELECT id from `DatabaseName`.`TableName` where author='" + Author + "' and title='" + Title + "' and publishDate='" + publishDate + "';";
	int check = retrieve("Documents", "id", "title", Title);

	return 0;
	
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
	// CALL insertSentence(DocumentID, words)
	return 0;
}

// Returns the string of the selected sentence. 
string StyleDatabase::getSentence(int sentenceID)
{
	// SELECT Tokens.Word FROM HMMtokenPaths 
	//    JOIN Tokens ON Tokens.TokenID = HMMtokenPaths.CurrentToken
	//    WHERE HMMtokenPaths.SentenceID = sentenceID
	//    ORDER BY HMMtokenPaths.TokenPathID  // this assumes the tokens were added in the database in order.
	return "";
}

// returns the number of instances (the count) in all documents of that series of 3 words.
//	Returns a list of the SentenceID’s for every occurance of that string. 
int StyleDatabase::getWordGroupCount(string prevWord, string currWord, string nextWord)
{
	//SELECT COUNT(CurrentToken) FROM HMMtokenPaths
	//	WHERE CurrentToken = currWord
	//	AND NextToken = nextWord
	//	AND PreviousToken = prevWord;

	return 0;
}

//	same as above but restricts the count to a specific style.
int StyleDatabase::getWordGroupCountByStyle(int StyleID, string prevWord, string currWord, string nextWord)
{
	//SELECT COUNT(CurrentToken) FROM HMMtokenPaths
	//	WHERE StyleID = styleID
	//	AND CurrentToken = currWord
	//	AND NextToken = nextWord
	//	AND PreviousToken = prevWord;

	return 0;
}

//Returns a list of all of the SentenceIDs where that combination of words exists in all documents of the selected style. 
vector<int> StyleDatabase::getWordGroupListByStyle(int StyleID, string prevWord, string currWord, string nextWord)
{
	//SELECT CurrentToken FROM HMMtokenPaths
	//	WHERE StyleID = styleID
	//	AND CurrentToken = currWord
	//	AND NextToken = nextWord
	//	AND PreviousToken = prevWord;

	vector<int> tokens;
	return tokens;
}
