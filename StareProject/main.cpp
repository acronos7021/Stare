using namespace std;

//#include <iostream>
//#include <vector>
//#include "CMDparser.h"
//#include "HMMengine.h"
#include "Console.h"
//#include "Database.h"

int main(int argc, char *argv[])
{
	//MetaData md = CMDparser::parseCMD(argc, argv);

	//HMMengine hGen = HMMengine(md);

	Console c;
	c.MessageLoop(argc, argv);




	//system("pause");
}