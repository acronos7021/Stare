using namespace std;


#include "CMDparser.h"
#include <sstream> 
#include <fstream>
//#include <algorithm>

bool CMDparser::parseCMD(vector<string> cmdList)
{
	//MetaData metaData;

	// Change command string to lower case
	//std::transform(cmdList[0].begin(), cmdList[0].end(), cmdList[0] , (int(*)(int))tolower);

	// Select the task
	if (cmdList[0] == "learn")
		Learn(cmdList);
	else if (cmdList[0] == "compare")
		Compare(cmdList);
	else if (cmdList[0] == "create")
		Create(cmdList);
	else if (cmdList[0] == "quit")
		// return false to end message loop
		return false;
	else 
	{
		cout << "'" << cmdList[0] << "'" << " is not a valid command." << endl;
		cout << "Type 'help' for a list of commands." << endl;
	}
	// load the file from the database and insert it into DocumentText
	return true;
}

void CMDparser::Learn(vector<string> cmdList)
{
	if (cmdList.size() != 5)
	{
		cout << "Invalid Parameters" << endl;
		cout << "example: learn [filename][Author][Title][PublishDate]";
		return;
	}
	MetaData metaData;
	metaData.DocumentText = ReadFile(cmdList[1]);
	metaData.action = ActionType::Learn;
	metaData.Author = cmdList[2];
	metaData.Title = cmdList[3];
	metaData.PublishDate = cmdList[4];

}
void CMDparser::Compare(vector<string> cmdList)
{
	if (cmdList.size() != 5)
	{
		cout << "Invalid Parameters" << endl;
		cout << "example: compare [filename][Author][Title][PublishDate]" << endl;
		return;
	}
	MetaData metaData;
	metaData.DocumentText = ReadFile(cmdList[1]);
	metaData.action = ActionType::Learn;
	metaData.Author = cmdList[2];
	metaData.Title = cmdList[3];
	metaData.PublishDate = cmdList[4];
}
void CMDparser::Create(vector<string> cmdList)
{
	if (cmdList.size() != 3)
	{
		cout << "Invalid Parameters" << endl;
		cout << "example: learn [filename][Author]" << endl;
		return;
	}

	MetaData metaData;
	metaData.DocumentText = ReadFile(cmdList[1]);
	metaData.action = ActionType::Learn;
	metaData.Author = cmdList[2];
}

string CMDparser::ReadFile(string fileName)
{
	using std::ifstream;

	ifstream t(fileName);
	std::stringstream buffer;

	if (!t.is_open())
	{
		cout << "can't open" << fileName << "file for input/n" << endl;
		return "";
	}
	else
	{
		buffer << t.rdbuf();
		return buffer.str();
	}

}
