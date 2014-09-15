using namespace std;

#include <iostream>
#include <vector>
#include "CMDparser.h"
#include "HMMengine.h"

int main(int argc, char *argv[])
{
	MetaData md = CMDparser::parseCMD(argc, argv);

	HMMengine hGen = HMMengine(md);

	system("pause");
}