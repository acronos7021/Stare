#include "PHPsocket.h"


PHPsocket::PHPsocket()
{
}

PHPsocket::~PHPsocket()
{
}

string PHPsocket::doCompare(Json::Value json)
{
	CMDparser cmd;
	string ID = json["clientID"].asString();
	int numb;
	istringstream(ID) >> numb;
	CompareResult result = cmd.compare(numb, json["documentText"].asString());
	return result.sentenceRankings[0].sourceSentences.SentenceStr;
}

Json::Value PHPsocket::parseJSON(string json)
{
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json,
		root,
		false);
	if (!parsedSuccess)
	{
		// Report failures and their locations 
		// in the document.
		cout << "Failed to parse JSON" << endl
			<< reader.getFormattedErrorMessages()
			<< endl;
		return NULL;
	}
	return root;
}

