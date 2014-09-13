using namespace std;


#include "CMDparser.h"

MetaData CMDparser::parseCMD(int argc, char *argv[])
{
	MetaData metaData;

	// for texting purposes, this prints all of the arguments to the console.
	for (int i = 0; i < argc; i++)
		cout << argv[i] << endl;

	// We need to do error checking and load the metaData here.  We will need to load the file if one is referenced.
	// example metaData
	metaData.action = ActionType::Create;
	metaData.Author = "Brian Davis";
	metaData.Title = "My Test Doc";
	metaData.PublishDate = "2014";
	metaData.DocumentText = "This will probably be loaded from a file instead of pulled directly from the command line";

	return metaData;
}