using namespace std;

#include "serverv2.h"
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
	serverv2 PHPconnect;
	PHPconnect.initEngine(&cmd);
	PHPconnect.startServer();
	Console c;
	c.MessageLoop(cmd, argc, argv);	

}