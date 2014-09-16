using namespace std;


#include "CMDparser.h"
#include <sstream> 
#include <fstream>

MetaData CMDparser::parseCMD(int argc, char *argv[])
{
	MetaData metaData;

	// Select the task
	if (argv[1] == "-learn")
	{
		//Load learn
		metaData.action = ActionType::Learn;
		metaData.Author = argv[3];
		metaData.Title = argv[4];
		metaData.PublishDate = argv[5];
	}
	else if (argv[1] == "-compare")
	{
		// Load compare
		metaData.action = ActionType::Compare;
		metaData.Author = argv[3];
		metaData.Title = argv[4];
		metaData.PublishDate = argv[5];
	}

	else if (argv[1] == "-create")
	{
		// Load create
		metaData.action = ActionType::Create;
		metaData.Author = argv[3];

	}
	else
	{
		// throw serious error and kill the app.
		// Brian needs to deal with this.  He's ignoring it for now.
	}

	// load the file from the database and insert it into DocumentText
	using std::ifstream;

	ifstream t(argv[2]);

	std::stringstream buffer;

	if (!t.is_open())
	{
		cout << "can't open" << argv[2] << "file for input/n";
	}
	buffer << t.rdbuf();

	metaData.DocumentText = buffer.str();

	return metaData;
}