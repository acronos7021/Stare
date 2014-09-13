using namespace std;

#include <iostream>
#include <vector>
#include "CMDparser.h"

int main(int argc, char *argv[])
{
	MetaData md = CMDparser::parseCMD(argc,argv);


	system("pause");
}