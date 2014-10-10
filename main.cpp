using namespace std;

//#include <iostream>
//#include <vector>
//#include "CMDparser.h"
//#include "HMMengine.h"
#include "Console.h"
#include "Database.h"
//#include "DataStructs.h"

//StyleDatabase& db = StyleDatabase();
//StyleDatabase& db;
//StyleDatabase& db;

int main(int argc, char *argv[])
{

    //db = StyleDatabase();
	//MetaData md = CMDparser::parseCMD(argc, argv);

	//HMMengine hGen = HMMengine(md);
	// Open the only copy of the database

//	db.open();
  //db.open();

	Console c;
	c.MessageLoop(argc, argv);


	//StyleDatabase test("aisql.db3");



	//system("pause");
}