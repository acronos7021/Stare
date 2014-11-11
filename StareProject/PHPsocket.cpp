#include <thread>
#include "PHPsocket.h"
#include "serverv2.h"

PHPsocket::PHPsocket()
{
}

PHPsocket::~PHPsocket()
{
}



string PHPsocket::jsonDecoder(string json)
{
	Json::Value jsonObject=parseJSON(json);
	string command = jsonObject["command"].asString();
	string output;

	if (command.compare("compare")==0){
		output = doCompare(jsonObject);
	}
	
	else if (command.compare("getStyles") == 0) {
	    output = getStyles();
	}
	else if (command.compare("checkCompare") == 0){
		string ID = jsonObject["clientID"].asString();
		int sessionID;
		istringstream(ID) >> sessionID;
		output = doCompare(jsonObject);
	}
	
	else if (command.compare("create")==0) {
		output = doCreate();
	}
	return output;
}

String PHPsocket::doCreate(Json::Value json) {
	
	cmd->compare(json["clientID"], json["style"], json["numberOfSentences"]);
	
	Json::Value outer;

   return "";	
}
String PHPsocket::getStyles() {
    
	vector<string> styles = cmd->getStyles();
	
	Json::Value outer;
	Json::Value inner;
	
	for(int i = 0; i < styles.size(); i++) {
	   inner.append(styles[i]);
	}
	outer["command"]="getStyles";
	outer["styles"] = inner;
	
	return outer.toStyledString();
}

string PHPsocket::doCompare(Json::Value json)
{
	CMDparser cmd;
	string ID = json["clientID"].asString();
	int sessionID;
	istringstream(ID) >> sessionID;
	CompareResult result = cmd.compare(sessionID, json["documentText"].asString());

	//Here I check if what compare returns is empty
	//TODO change this to the boolean.
	if (result.documentCertainties.empty()){
		int progress = cmd.checkCompareStatus(sessionID);
		Json::Value output = formCheckCompareReturn(progress);
		return output.toStyledString();
	}

	//make JSON here, to send back to the browser

	Json::Value compare;
	compare["command"] = "checkCompare";
	//TODO change this to output overall certainty, when I know how.
	compare["overallCertainty"] = "N/A";
	for (unsigned int i = 0; i < result.sentenceRankings.size(); ++i)
	{
		Json::Value rankingObj;
		Json::Value origSnip;
		Json::Value dataBaseSnip;

		origSnip.append(result.sentenceRankings[i].sourceSentences.PrevSentenceStr);
		origSnip.append(result.sentenceRankings[i].sourceSentences.SentenceStr);
		origSnip.append(result.sentenceRankings[i].sourceSentences.NextSentenceStr);
		rankingObj["origSnip"] = origSnip;

		dataBaseSnip.append(result.sentenceRankings[i].foundSentences.PrevSentenceStr);
		dataBaseSnip.append(result.sentenceRankings[i].foundSentences.SentenceStr);
		dataBaseSnip.append(result.sentenceRankings[i].foundSentences.NextSentenceStr);
		rankingObj["dataBaseSnip"] = dataBaseSnip;

		rankingObj["documentTitle"] = result.sentenceRankings[i].foundDocumentName;
		rankingObj["certainty"] = result.sentenceRankings[i].certainty;
		compare["ranking"].append(rankingObj);
	}

	return compare.toStyledString();
}


Json::Value PHPsocket::formCheckCompareReturn(int status)
{
	Json::Value compare;
	compare["command"] = "checkCompare";
	compare["status"] = status;
	return compare;
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

