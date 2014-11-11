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
	
	
	MetaData md;
	md.DocumentText = "../Documents/AMidsummerNightsDream.txt";
	md.Author = "Shakespere";
	md.Title = "A Midnight Summer Dream" ;
	md.PublishDate = "1605" ;
	cmd.hmm.learnFromFile(md);

	cmd.hmm.dataBase.initTables();
	
	serverv2 PHPconnect;
	PHPconnect.initEngine(&cmd);
	PHPconnect.startServer();
	Console c;

	
	c.MessageLoop(cmd, argc, argv);	

}


/*
 * 	
	MetaData md;
	md.DocumentText = "../Documents/AMidsummerNightsDream.txt";
	md.Author = "Shakespere";
	md.Title = "A Midnight Summer Dream" ;
	md.PublishDate = "1605" ;
	cmd.hmm.compareWithFile(cmd.hmm,md);
	*/