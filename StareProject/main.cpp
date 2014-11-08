using namespace std;

//#include <iostream>
//#include <vector>
#include "CMDparser.h"
//#include "HMMengine.h"
#include "Console.h"
#include "CMDparser.h"
//#include "DataStructs.h"

//StyleDatabase& db = StyleDatabase();

int main(int argc, char *argv[])
{
	CMDparser cmd;
	Console c(cmd);
	//HeipClass heipClass;
	//heipClass.heipMethod(cmd)   // on the class side use  helpMethod(CMDparser &cmdParser);
	c.MessageLoop(argc, argv);

}