#include "Console.h"
#include "CMDparser.h"
#include "Tokenizer.h"


Console::Console()
{
}


Console::~Console()
{
}

void Console::MessageLoop(int argc, char *argv[])
{
	std::cout << "Welcome to the Stare plagerism detector." << endl;
	CMDparser cmdParser;
	if (argc > 1)
	{
		// a command was entered on the command line that started the program
		// go ahead and process it.
		vector<string> commandList;
		for (int i = 1; i < argc; i++)
			commandList.push_back(argv[i]);
		cmdParser.parseCMD(commandList);
	}
	string cmdStr;
	do
	{
		std::cout << ">>  " ;
		std::cin >> cmdStr;
	} while (cmdParser.parseCMD(Tokenizer(cmdStr).getNextSentence()));

}
