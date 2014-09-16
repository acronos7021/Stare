using namespace std;


#include "CMDparser.h"
#include <sstream> 
#include <fstream>

MetaData CMDparser::parseCMD(int argc, char *argv[])
{
	MetaData metaData;


	// for texting purposes, this prints all of the arguments to the console.
	for (int i = 0; i < argc; i++)
		cout << argv[i] << endl;

	//// Here is an example of how  to use the MetaData struct
	if (argv[1] == "-learn")
	{
		//do learn stuff
		metaData.action = ActionType::Learn;
		metaData.Author = argv[3];//"Brian Davis";
		metaData.Title = argv[4];// "My Test Doc";
		metaData.PublishDate = argv[5];// "2014";
	}
	else if (argv[1] == "-compare")
	{
		// do compare stuff
		metaData.action = ActionType::Learn;
		metaData.Author = argv[3];//"Brian Davis";
		metaData.Title = argv[4];// "My Test Doc";
		metaData.PublishDate = argv[5];// "2014";
	}

	else if (argv[1] == "-create")
	{
		// do compare stuff
		metaData.action = ActionType::Learn;
		metaData.Author = argv[3];//"Brian Davis";

	}
	else
		// throw serious error and kill the app.

		// use argv[2] to read the file and load it into metaData.DocumentText in all three versions
		using std::ifstream;
	ifstream t(argv[2]);

	std::stringstream buffer;

	if (!t.is_open())
	{
		cout << "can't open" << argv[2] << "file for input/n";
	}
	buffer << t.rdbuf();


	metaData.DocumentText = buffer.str();

	metaData.action = ActionType::Create;

	metaData.DocumentText = "This will probably be loaded from a file instead of pulled directly from the command line";
	

	return metaData;
}