using namespace std;



#include <sstream> 
#include <fstream>
#include <iostream>
#include <vector>
#include "Stopwatch.h"
#include "Database.h"
#include "CMDparser.h"
#include "Tokenizer.h"
#include "HMMengine.h"
#include "DataStructs.h"
#include "PHPsocket.h"


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
	else if ((cmdList[0] == "Create") && (cmdList[1] == "Database"))
		db.CreateDatabase();
	else if (cmdList[0] == "Execute")
		Execute(cmdList[1]);
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
	metaData.DocumentText = cmdList[1]; // ReadFile(cmdList[1]);
	metaData.action = ActionType::Learn;
	metaData.Author = cmdList[2];
	metaData.Title = cmdList[3];
	metaData.PublishDate = cmdList[4];
	cout << "Learning document -> " << metaData.Author << " : " << metaData.Title;
	HMMengine hmm;
	hmm.learn(metaData);
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
	metaData.DocumentText = cmdList[1]; // ReadFile(cmdList[1]);
	metaData.action = ActionType::Compare;
	metaData.Author = cmdList[2];
	metaData.Title = cmdList[3];
	metaData.PublishDate = cmdList[4];
	cout << "Comparing document -> " << metaData.Author << " : " << metaData.Title;
	HMMengine hmm;
	hmm.compare(metaData);
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
	HMMengine hmm;
	hmm.create(metaData);
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

void CMDparser::Execute(string file)
{
	string line;
	vector<string> cmdLst;
	string fileWithExtension = file + ".stare";
	string commandString = ReadFile(fileWithExtension);
	if (commandString != "")
	{
		istringstream commandStream(commandString);
		while (getline(commandStream, line))
		{
			cout << "<< " << line << endl;
			cmdLst = CMDparser::getCommands(line);
			parseCMD(cmdLst);
			cout << endl << endl;
		}
	}
}

vector<string> CMDparser::getCommands(string cmdStr)
{
	vector<string> words;
	stringstream ss;

	// I was too lazy to implement a full state machine.  However...
	// Uses 3 states to determine the action at a character position.  They are:
	// inWord==false, inQuotes==false :  this state means that any non Alphanums are dumped until a proper alphanum is encountered
	// inWord==true, inQuotes==false  :  This state meant that any alphanums are added to the current word until a non-alphanum is encountered
	// inWord==true, inQuotes==true  :  Anything, whether alphanum or not, is added to the current word until another '"' is encountered.

	bool inWord = false;   // if inWord, all alphanumeric 
	bool inQuotes = false; // if inQuotes is true, all characters are recorded into the current ss until another quote is encountered.

	for (unsigned int i = 0; i < cmdStr.size(); i++)
	{
		if (!inWord)
		{
			if (isAlphaNumeric(cmdStr[i]))
			{
				// start word
				ss << cmdStr[i];
				inWord = true;
			}
			else if (cmdStr[i] == '"')
			{
				// start quotes
				inWord = true;
				inQuotes = true;
			}
		}
		else
		{
			// inWord is true
			if (inQuotes)
			{
				if (cmdStr[i] != '"')
					ss << cmdStr[i];
				else
				{
					// the quotes have ended
					words.push_back(ss.str());
					ss.str("");  // empty ss
					inWord = false;
					inQuotes = false;

				}
			}
			else
			{
				if (isAlphaNumeric(cmdStr[i]))
					ss << cmdStr[i];
				else
				{
					// the word has ended
					words.push_back(ss.str());
					ss.str("");  // empty ss
					inWord = false;
				}

			}
		}
	}

	// The last word can end with the end of the string.  
	// It would not be detected then, so add it here.
	if (inWord)
		words.push_back(ss.str());

	return words;
}

bool CMDparser::isAlphaNumeric(char c)
{
	if (((c >= 'a') && (c <= 'z')) ||
		((c >= 'A') && (c <= 'Z')) ||
		((c >= '0') && (c <= '9')))
		return true;
	else
		return false;
}

/******************************   PHPconnector functions  ************************************/


/******************************   PHPconnector functions  ************************************/
string CMDparser::getSentences(int rangeStart, int rangeEnd)
{
	return "This is sentence 3.  This is sentence 4.";
}

string CMDparser::getDocument(int DocumentID)
{
	return"This is sentence 1.  This is sentence 2. This is sentence 3.  This is sentence 4.";
}


void CMDparser::learn(string author, string title, string date, string text)
{
	HMMengine hmm;
	hmm.compare(MetaData(author, title, date, text));
	string sam = "good";
}

CompareResult CMDparser::compare(int clientID, string text)
{
	//HMMengine hmm;
	//hmm.compare(MetaData("", "", "", text));
	vector<SentenceRanking> test;

	SentenceBlob sbSource;
	sbSource.NextNextSentenceStr = "Sentence 9";
	sbSource.NextSentenceStr = "Sentence 8";
	sbSource.SentenceStr = "Sentence 7";
	sbSource.PrevSentenceStr = "Sentence 6";
	sbSource.PrevPrevSentenceStr = "Sentence 5";
	sbSource.SentenceID = 7;

	SentenceBlob sbFound;
	sbFound.NextNextSentenceStr = "Sentence 9";
	sbFound.NextSentenceStr = "Sentence 8";
	sbFound.SentenceStr = "Sentence 7";
	sbFound.PrevSentenceStr = "Sentence 6";
	sbFound.PrevPrevSentenceStr = "Sentence 5";
	sbFound.SentenceID = 7;

	test.push_back(SentenceRanking("Dickens", "A tale of two cities", sbSource, sbFound, .57));
	test.push_back(SentenceRanking("Mark Twain", "Huckleberry Finn", sbSource, sbFound, .57));

	StyleCertaintyItem style1;
	style1.certainty = .73;
	style1.StyleID = 2;
	style1.StyleName = "Mark Twain";

	vector<StyleCertaintyItem> ci;
	ci.push_back(style1);

	CompareResult cr;
	cr.documentCertainties = ci;
	cr.sentenceRankings = test;

	return cr;
}

string CMDparser::create(int clientID, string author, int numOfSentences)
{
	return "Now is the time for all good men to come to the aid of their country.";
}

int CMDparser::checkCompareStatus(int clientID)  // done is 100
{
	return 25;
}
int CMDparser::checkCreateStatus(int clientID)     // done is 100
{
	return 25;
}

vector<MetaData> CMDparser::getDocuments()
{
	MetaData md("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt");
	std::vector<MetaData> test;
	test.push_back(md);
	test.push_back(md);
	test.push_back(md);
	return test;

}

std::vector<MetaData> CMDparser::getStyles()
{
	MetaData md("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt");
	std::vector<MetaData> test;
	test.push_back(md);
	test.push_back(md);
	test.push_back(md);
	return test;
}



/**********************************************************************************************/

void CMDparser::Brandon(vector<string> cmdParams)
{
	//cout << "The int on our platform is " << (sizeof(int)*8) << " bits" << endl;
	//cout << "The long on our platform is " << (sizeof(long)* 8) << " bits" << endl; 
	//cout << "The long long on our platform is " << (sizeof(long long)* 8) << " bits" << endl;
	//StyleDatabase& test = StyleDatabase::getInstance();
	//test.open("BrianAIsql.db3");
	db.clearDatabase();
	db.insertAuthor("Brian");
	int StyleID = db.retrieveAuthorStyleID("Brian");
	//test.insertDocument(StyleID, "Brian book", "1996");
	db.retrieve("Styles", "Author", "Author", "Sam");
	int DocumentID = db.insertDocument("Sam", "Sams Book", "1900");  // test new path
	DocumentID = db.insertDocument("Brian", "Brians other book.", "1998"); // test same author, new book
	DocumentID = db.insertDocument("Brian", "Brian book", "1996"); // test same author, same book.
	vector<string> sentVect1;
	sentVect1.push_back("This");
	sentVect1.push_back("is");
	sentVect1.push_back("a");
	sentVect1.push_back("test");
	db.insertSentence(DocumentID, sentVect1);
	db.insertSentence(DocumentID, sentVect1); // test duplicate words.
	vector<string> sentVect2;
	sentVect2.push_back(".");
	db.insertSentence(DocumentID, sentVect2); // test short sentence.
}

void CMDparser::Blake(vector<string> cmdParams)
{
	string json = "{ \"clientID\":\"789\", \"command\" : \"compare\", \"documentText\" : \"the document text here\" }";
	PHPsocket test;
	cout << test.doCompare(test.parseJSON(json)) << endl;
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
	HMMengine hmm;
	Stopwatch sw;


	//sw.start();
	//Tokenizer tokenizer = Tokenizer();
	//std::vector <std::vector<int>> Midsummer = tokenizer.tokenizeDoc("../StareProject/Documents/AMidsummerNightsDream.txt");
	//sw.end();

	//int tokenizerTime = sw.getTimeInMicroseconds();

	////	vector<string> sentenceVect;
	//sw.start();

	//int docID = db.insertDocument("Shakespere", "A Midnight Summer Dream", "a long long ago");
	//sw.end();
	//int dbDocTime = sw.getTimeInMicroseconds();

	//sw.start();
	//db.insertDocumentText(docID, Midsummer);
	//sw.end();

	//int dbTextTime = sw.getTimeInMicroseconds();

	//  Add more books
	//db.clearDatabase();

	//sw.start();
	//cout << "Loading Henry V" << endl;
	//hmm.learn(MetaData("Shakespere", "Henry V", "1619", "../StareProject/Documents/HenryV.txt"));
	//cout << "Loading Romeo and Juliet" << endl;
	//hmm.learn(MetaData("Shakespere", "Romeo and Juliet", "1597", "../StareProject/Documents/RomeoAndJuliet.txt"));
	//cout << "Loading A Tale of Two Cities" << endl;
	//hmm.learn(MetaData("Charles Dickens", "A Tale of Two Cities", "1859", "../StareProject/Documents/ATaleOfTwoCities.txt"));
	//cout << "Loading Great Expectation" << endl;
	//hmm.learn(MetaData("Charles Dickens", "Great Expectations", "1860", "../StareProject/Documents/GreatExpectations.txt"));
	//sw.end();
	//int learnTime = sw.getTimeInMicroseconds();

	hmm.compare(MetaData("Shakespere", "A Midnight Summer Dream", "1605", "../StareProject/Documents/AMidsummerNightsDream.txt"));
}

	//do
	//{
	//	sentenceVect = tokenizer.getNextSentence();
	//	db.insertSentence(docID, sentenceVect);
	//} while (sentenceVect.size() > 0);
	//sw.end();

	//int dbTime = sw.getTimeInMicroseconds();

	//int sentID = 
	//string sent = db.getSentence(sentID);
	//Tokenizer tokenizer2 = Tokenizer("../StareProject/Documents/AChristmasCarol.txt");
	//tokenizer2.tokenizeDoc();
	//int docID2 = db.insertDocument("Charles Dickens", "A Christmas Carol", "1864");
	//do
	//{
	//	sentenceVect = tokenizer2.getNextSentence();
	//	db.insertSentence(docID2, sentenceVect);
	//} while (sentenceVect.size() > 0);


//}