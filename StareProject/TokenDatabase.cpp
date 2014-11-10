#include "TokenDatabase.h"


TokenDatabase::TokenDatabase()
{
	lastToken = 0;
	lastAllocatedTokenID = 0;
	currentAllocatedTokenID = 0;
	LoadTokenMap();
}


TokenDatabase::~TokenDatabase()
{
}

sqlite3* TokenDatabase::openDB()
{
	// Convert the dbName string to a format sqlite3_open needs
	//const char * c = dbName.c_str();
	sqlite3 *db;
	if (sqlite3_open("AIsql.db3", &db) != SQLITE_OK)
		throw std::runtime_error("SQLite database failed to open");
	return db;
}

void TokenDatabase::close(sqlite3 *db)
{
	sqlite3_close(db); // Try to close the database
}

void TokenDatabase::LoadTokenMap()
{
	//char* errorMessage;

	sqlite3_stmt *select_statement;
	sqlite3* db = openDB();
	std::string selectTokenStr = "SELECT TokenID,Word FROM Tokens WHERE TokenID >= " + std::to_string(lastToken); // There is a bug here.  If another process gets tokens allocated and then adds them, they will not show up here.
	if (sqlite3_prepare(db, selectTokenStr.c_str(), -1, &select_statement, 0) == SQLITE_OK)
	{
		int checkRow;
		while (true)
		{
			checkRow = sqlite3_step(select_statement);
			if (checkRow == SQLITE_ROW)
			{
				int tokenID = sqlite3_column_int(select_statement, 0);
				std::string word = (char*)sqlite3_column_text(select_statement, 1);
				tokenMap.insert(std::pair<std::string, int>(word, tokenID));
				reverseTokenMap.insert(std::pair<int, std::string>(tokenID, word));
				if (tokenID > lastToken) lastToken = tokenID;
			}
			else break;
		}
	}
	close(db);
}

int TokenDatabase::GetToken(std::string word)
{

	int ret = 0;
	std::map<std::string, int>::iterator it = tokenMap.find(word);
	if (it == tokenMap.end())
	{
		// the token does not exist in the database
		// so add it to the temp list and insert it into the map
		int nextTokenID = GetNextTokenID();
		std::pair<std::string, int> tokenPair = std::pair<std::string, int>(word, nextTokenID);
		tokenMap.insert(tokenPair);
		reverseTokenMap.insert(std::pair<int, std::string>(nextTokenID, word));
		tokenCache.push_back(tokenPair);
		if (nextTokenID > lastToken) lastToken = nextTokenID;
		ret =  nextTokenID;
	}
	else
		ret =  it->second;
	return ret;
}

std::string TokenDatabase::GetString(int TokenID)
{
	std::string word;
	try
	{
		word = reverseTokenMap.find(TokenID)->second;
	}
	catch (...)
	{
		word = "";
	}

	return word;
}

void TokenDatabase::FlushTokenCache()
{
	sqlite3* db = openDB();
	if (tokenCache.size() > 0)
	{
		//char* errorMessage;
		sqlite3_stmt *stmt;

		sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);

		// setup pre-prepared sql statements.
		char buffer[] = "INSERT INTO Tokens VALUES (?1, ?2)";

		//sqlite3_prepare_v2(db, buffer, strlen(buffer), &stmt, NULL);
		sqlite3_prepare_v2(db, "INSERT INTO Tokens VALUES (?1, ?2)", -1, &stmt, NULL);

		for (std::vector<std::pair<std::string, int>>::iterator it = tokenCache.begin(); it != tokenCache.end(); ++it)
		{
			// bind to prepared statement. 
			sqlite3_bind_int(stmt, 1, it->second);
			sqlite3_bind_text(stmt, 2, it->first.c_str(), it->first.size(), 0);
			sqlite3_step(stmt);
			sqlite3_reset(stmt);
		}
		sqlite3_finalize(stmt);
		sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
		tokenCache.clear();
	}
	close(db);
}

int TokenDatabase::GetNextTokenID()
{
	// get tokenIDs in 100 allocations.  Only ask for a new allocation when it is needed.
	if (currentAllocatedTokenID == lastAllocatedTokenID)
	{
		// get new batch of tokens from the server
		//char* errorMessage;
		sqlite3* db = openDB();
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
				std::string updateStr = "UPDATE Variables SET LastTokenID = " + std::to_string(lastAllocatedTokenID);
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
			sqlite3_finalize(select_statement);
		}
		sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
		close(db);
	}
	return ++currentAllocatedTokenID;
}
