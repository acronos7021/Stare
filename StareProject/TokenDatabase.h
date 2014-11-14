#pragma once
#include "sqlite3.h"
#include <string>
#include <map>
//#include <unordered_map>
#include <vector>

class TokenDatabase
{
public:
	TokenDatabase();
	~TokenDatabase();


	int insert(sqlite3* db, std::string q);
	sqlite3* openDB();
	void close(sqlite3 *db);
	bool isOpen;


	std::map<std::string, int> tokenMap;
	std::map<int, std::string> reverseTokenMap;
	int lastToken;
	int currentAllocatedTokenID, lastAllocatedTokenID;
	std::vector<std::pair<std::string, int>> tokenCache;

	int GetToken(std::string word);

	void LoadTokenMap( );
	std::string GetString(int TokenID);
	void FlushTokenCache();
	int GetNextTokenID();
};

