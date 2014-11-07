using namespace std;

//#include <iostream>
//#include <vector>
#include "CMDparser.h"
//#include "HMMengine.h"
#include "Console.h"
#include "Database.h"
//#include "DataStructs.h"

StyleDatabase& db = StyleDatabase();

int main(int argc, char *argv[])
{
	db.open();

	CMDparser cmdParser;
	//HeipClass heipClass;
	//heipClass.heipMethod(cmdParser)   // on the class side use  helpMethod(CMDparser &cmdParser);

	Console c;
	c.MessageLoop(cmdParser, argc, argv);
}