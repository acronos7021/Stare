#include <thread>
#include "PHPsocket.h"
#include "serverv2.h"

PHPsocket::PHPsocket(CMDparser* cmd)
{
  this->cmd = cmd;	
}

PHPsocket::~PHPsocket()
{
}


std::string PHPsocket::sanitize(std::string in_str)
{
    std::stringstream out_str;
    for (std::string::size_type i = 0; i < in_str.length(); ++i)
    {
	switch (in_str[i]) {
	case '"':
	out_str << "\\\"";
	break;
	case '/':
	out_str <<  "\\/";
	break;
	case '\b':
	out_str <<  "\\b";
	break;
	case '\f':
	out_str <<  "\\f";
	break;
	case '\n':
	out_str <<  "\\n";
	break;
	case '\r':
	out_str << "\\r";
	break;
	case '\t':
	out_str <<  "\\t";
	break;
	case '\\':
	out_str <<  "\\\\";
	break;
	default:
	out_str << in_str[i];
	break;
	}
    }
    return out_str.str();
}

enum State {ESCAPED,UNESCAPED};

std::string PHPsocket::desanitize(std::string in_str)
{
    State s = UNESCAPED;
    std::string out_str;
    out_str.reserve(in_str.length());
    for (std::string::size_type i = 0; i < in_str.length(); ++i)
    {
	switch(s)
	{
	    case ESCAPED:
	    {
		switch(in_str[i])
		{
		    case '"':
		    out_str += '\"';
		    break;
		    case '/':
		    out_str += '/';
		    break;
		    case 'b':
		    out_str += '\b';
		    break;
		    case 'f':
		    out_str += '\f';
		    break;
		    case 'n':
		    out_str += '\n';
		    break;
		    case 'r':
		    out_str += '\r';
		    break;
		    case 't':
		    out_str += '\t';
		    break;
		    case '\\':
		    out_str += '\\';
		    break;
		    default:
		    out_str += in_str[i];
		    break;
		}
		s = UNESCAPED;
		break;
	    }
	    case UNESCAPED:
	    {
		switch(in_str[i])
		{
		    case '\\':
		    s = ESCAPED;
		    break;
		    default:
		    out_str += in_str[i];
		    break;
		}
	    }
	}
    }
    return out_str;
}


std::string PHPsocket::jsonDecoder(std::string json)
{
	Json::Value jsonObject=parseJSON(json);
	std::string command = jsonObject["command"].asString();
	std::string output;

	if (command.compare("compare")==0){
		output = doCompare(jsonObject);
	}
	
	else if (command.compare("getStyles") == 0) {
	    output = getStyles();
	}
	else if (command.compare("checkCompare") == 0){
		std::string ID = jsonObject["clientID"].asString();
		int sessionID;
		istringstream(ID) >> sessionID;
		output = doCompare(jsonObject);
	}
	
	else if (command.compare("create")==0) {
		output = doCreate(jsonObject);
	}
	
	return output;
}

std::string PHPsocket::doCreate(Json::Value json) {
	
	CreateResult result = cmd->create(json["clientID"].asInt(), json["style"].asString(), json["numberOfSentences"].asInt());
	std::string text = result.newDocument;
	
	
   return text;	
}
std::string PHPsocket::getStyles() {
    
	vector<std::string> styles = cmd->getStyles();
	
	Json::Value outer;
	Json::Value inner;
	
	for(int i = 0; i < styles.size(); i++) {
	   inner.append(styles[i]);
	}
	outer["command"]="getStyles";
	outer["styles"] = inner;
	
	return outer.toStyledString();
}

std::string PHPsocket::doCompare(Json::Value json)
{
	std::string ID = json["clientID"].asString();
	int sessionID;
	istringstream(ID) >> sessionID;
	CompareResult result = cmd->compare(sessionID, json["documentText"].asString());

	//Here I check if what compare returns is empty
	//TODO change this to the boolean.
	if (result.percentComplete<100){
		int progress = cmd->checkCompareStatus(sessionID);
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

Json::Value PHPsocket::parseJSON(std::string json)
{
    Json::StyledWriter styledWriter;
	Json::Value root;
	Json::Reader reader;
    bool parsingSuccessful = reader.parse(json, root);
    if (parsingSuccessful)
    {
        std::cout << styledWriter.write(root) << std::endl;
    }
	return root;
}

