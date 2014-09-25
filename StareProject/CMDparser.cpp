using namespace std;



#include <sstream> 
#include <fstream>
#include "Stopwatch.h"
#include "Database.h"
#include "CMDparser.h"
#include "Tokenizer.h"


bool CMDparser::parseCMD(vector<string> cmdList)
{
	//MetaData metaData;

	// Change command string to lower case
	//std::transform(cmdList[0].begin(), cmdList[0].end(), cmdList[0] , (int(*)(int))tolower);
	if (cmdList.size() <= 0)
		return true;  // The string was empty.

	// Select the task
	if (cmdList[0] == "learn")
		Learn(cmdList);
	else if (cmdList[0] == "compare")
		Compare(cmdList);
	else if (cmdList[0] == "create")
		Create(cmdList);
	else if (cmdList[0] == "Brandon")
		Brandon(cmdList);
	else if (cmdList[0] == "Blake")
		Blake(cmdList);
	else if (cmdList[0] == "Sam")
		Sam(cmdList);
	else if (cmdList[0] == "Hiep")
		Hiep(cmdList);
	else if (cmdList[0] == "Leven")
		Leven(cmdList);
	else if (cmdList[0] == "Brian")
		Brian(cmdList);
	else if (cmdList[0] == "help")
	{
		cout << "The commands are : 'quit', 'learn', 'compare', 'create', 'help'" << endl;
		cout << "For help on a specific command, type the command without arguments." << endl;
		cout << "All commands are case sensitive." << endl;
	}
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
		cout << "'learn' adds the document with the selected params to the database." << endl;
		if (cmdList.size() != 1) // it is not a request for help
			cout << "Invalid Parameters" << endl;
		cout << "example: learn [filename][Author][Title][PublishDate]" << endl;
		cout << "All arguements should be in quotes." << endl;
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
		cout << "'compare' uses the selected document pointed at by the params and" << endl;
		cout << "compares it to the rest of the documents in the database" << endl;
		cout << "to try to recognize the style of the selected document." << endl;
		if (cmdList.size() != 1) // it is not a request for help
			cout << "Invalid Parameters" << endl;
		cout << "example: compare [filename][Author][Title][PublishDate]" << endl;
		cout << "All arguements should be in quotes." << endl;
		return;
	}
	MetaData metaData;
	metaData.DocumentText = ReadFile(cmdList[1]);
	metaData.action = ActionType::Compare;
	metaData.Author = cmdList[2];
	metaData.Title = cmdList[3];
	metaData.PublishDate = cmdList[4];
}
void CMDparser::Create(vector<string> cmdList)
{
	if (cmdList.size() != 3)
	{
		if (cmdList.size() != 1) // it is not a request for help
			cout << "Invalid Parameters" << endl;
		cout << "example: learn [filename][Author]" << endl;
		cout << "All arguements should be in quotes." << endl;
		return;
	}

	MetaData metaData;
	metaData.DocumentText = ReadFile(cmdList[1]);
	metaData.action = ActionType::Create;
	metaData.Author = cmdList[2];
}

string CMDparser::ReadFile(string fileName)
{
	using std::ifstream;

	ifstream t(fileName);
	std::stringstream buffer;

	if (!t.is_open())
	{
		cout << "can't open '" << fileName << "' file for input" << endl;
		return "";
	}
	else
	{
		buffer << t.rdbuf();
		return buffer.str();
	}

}

void CMDparser::Brandon(vector<string> cmdParams)
{
	//cout << "The int on our platform is " << (sizeof(int)*8) << " bits" << endl;
	//cout << "The long on our platform is " << (sizeof(long)* 8) << " bits" << endl; 
	//cout << "The long long on our platform is " << (sizeof(long long)* 8) << " bits" << endl;
	StyleDatabase test("BrianAIsql.db3");
	test.clearDatabase();
	test.insertAuthor("Brian");
	int StyleID = test.retrieveAuthorStyleID("Brian");
	//test.insertDocument(StyleID, "Brian book", "1996");
	test.retrieve("Styles", "Author", "Author", "Sam");
	int DocumentID = test.insertDocument("Sam", "Sams Book", "1900");  // test new path
	DocumentID = test.insertDocument("Brian", "Brians other book.", "1998"); // test same author, new book
	DocumentID = test.insertDocument("Brian", "Brian book", "1996"); // test same author, same book.
	vector<string> sentVect1;
	sentVect1.push_back("This");
	sentVect1.push_back("is");
	sentVect1.push_back("a");
	sentVect1.push_back("test");
	test.insertSentence(DocumentID, sentVect1);
	test.insertSentence(DocumentID, sentVect1); // test duplicate words.
	vector<string> sentVect2;
	sentVect2.push_back(".");
	test.insertSentence(DocumentID, sentVect2); // test short sentence.
}

void CMDparser::Blake(vector<string> cmdParams)
{

}

void CMDparser::Sam(vector<string> cmdParams)
{

}

void CMDparser::Hiep(vector<string> cmdParams)
{

}

void CMDparser::Leven(vector<string> cmdParams)
{

}

void CMDparser::Brian(vector<string> cmdParams)
{
	Stopwatch sw;
	StyleDatabase db("AIsql.db3");
	db.clearDatabase();
//	string doc = ReadFile();

	sw.start();
	Tokenizer tokenizer = Tokenizer("../StareProject/Documents/AMidsummerNightsDream.txt");
	tokenizer.tokenizeDoc();
	sw.end();

	int tokenizerTime = sw.getTimeInMicroseconds();

	vector<string> sentenceVect;
	sw.start();

	int docID = db.insertDocument("Shakespere", "A Midnight Summer Dream", "a long long ago");
	do
	{
		sentenceVect = tokenizer.getNextSentence();
		db.insertSentence(docID, sentenceVect);
	} while (sentenceVect.size() > 0);
	sw.end();

	int dbTime = sw.getTimeInMicroseconds();

	//int sentID = 
	//string sent = db.getSentence(sentID);
	Tokenizer tokenizer2 = Tokenizer("../StareProject/Documents/AChristmasCarol.txt");
	tokenizer2.tokenizeDoc();
	int docID2 = db.insertDocument("Charles Dickens", "A Christmas Carol", "1864");
	do
	{
		sentenceVect = tokenizer2.getNextSentence();
		db.insertSentence(docID2, sentenceVect);
	} while (sentenceVect.size() > 0);


}