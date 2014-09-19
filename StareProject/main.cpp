using namespace std;

#include <iostream>
#include <vector>
//#include "CMDparser.h"
//#include "HMMengine.h"
//#include "Console.h"
#include "Database.h"

int main(int argc, char *argv[])
{
	//MetaData md = CMDparser::parseCMD(argc, argv);

	//HMMengine hGen = HMMengine(md);

	/*Console c;
	c.MessageLoop(argc, argv);*/

	StyleDatabase test;
	int t = test.insertDocument("Test", "Test", "Test");

	vector<string> testVec;
	testVec.push_back("This");
	testVec.push_back("is");
	testVec.push_back("a");
	testVec.push_back("test");
	test.insertSentence(t, testVec);


	system("pause");
}