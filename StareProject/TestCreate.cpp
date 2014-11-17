#include "TestCreate.h"



TestCreate::TestCreate()
{
}


TestCreate::~TestCreate()
{
}

int TestCreate::rand()
{
	return randList[randIndex++];
}

bool TestCreate::isWordToken(int wordID, int styleID)
{
	return isWordTokenStateTable[isWordTokenStateTableIndex++];
}

std::vector<WordNextCountStruct> TestCreate::getNextToken(int curr, int style)
{
	return getNextTokenTable[curr][style];
}

std::string TestCreate::rebuildSent(std::vector<int> in)
{
	return "This is short sentence";
}


bool TestCreate::checkPunctuation(char c)
{

	if (c == '.') return true;
	if (c == '!') return true;
	if (c == '?') return true;
	return false;
}


void TestCreate::doTest()
{
	getNextTokenTable.resize(501);
	for (int i = 0; i < 501; i++)
	{
		getNextTokenTable[i].resize(4);
	}


	std::cout << std::endl << "Path 1 test" << std::endl;
	tokenMapSize = 3;
	createDoc(1, 3);
	tokenMapSize = 500;
	createDoc(1, 0);

	std::cout << std::endl << "Path 2 tests - I" << std::endl;
	// rand < 257
	// rand < 128
	tokenMapSize = 500;
	isWordTokenStateTableIndex = 0;
	isWordTokenStateTable = { false, true };
	randList = { 70, 30, 16,27,200,13,37,140 };
	randIndex = 0;
	//isWordTokenStateTable[randList[0]][2] = false;
	std::vector<WordNextCountStruct> nextTokenList;
	getNextTokenTable[randList[0]][2] = nextTokenList;
	createDoc(1, 1);

	std::cout << std::endl << "Path 3 tests - A" << std::endl;
	tokenMapSize = 500;
	randList[0] = 200;
	isWordTokenStateTableIndex = 0;
	randIndex = 0;
	//isWordTokenStateTable[randList[0]][2] = false;
	getNextTokenTable[randList[0]][2] = nextTokenList;
	createDoc(1, 1);



	std::cout << std::endl << "Path 4 tests" << std::endl;



	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;
	std::cout << std::endl << "Path 1 tests" << std::endl;


}



std::string TestCreate::createDoc(int styleID, int length)
{
	std::string s = "";
	std::vector<int> tmp;
	srand(time(0)*1.239857128012);

	int random;
	int prev = 0;
	std::cout << "1 ";
	int maxTok = tokenMapSize;

	std::cout << "2 ";
	int count = 0;
	bool lock = false;

	std::cout << "3 ";
	while (count < length && maxTok > 256)
	{
		std::cout << "4 ";
		random = (int)(rand() % maxTok);
		if (prev == random)
		{
			lock = false;
		}
		else
		{
			prev = random;
			lock = true;
		}
		while (lock)
		{
			std::cout << "5 ";
			if (random <= 256)
			{
				std::cout << "6 ";

				if (random < 128)
				{
					std::cout << "7 ";
					random = 73; // I
				}
				else
				{
					std::cout << "8 ";
					random = 65; // A
				}
			}
			std::cout << "9 ";
			if (isWordToken(random, styleID) == true)
			{
				std::cout << "10 ";
				tmp = createHelper(styleID, random);
				std::cout << "11 ";
				s += rebuildSent(tmp);
				s += " ";
				std::cout << "12 ";
				lock = false;
				count++;
			}
			else
			{
				std::cout << "13 ";
				lock = false;
				//random = (int)(rand() % maxTok);
			}
		}//end of inner loop
	}//end of outer loop
	std::cout << "Exit" <<std::endl;
	return s;
} //end of createDoc2()



std::vector<int> TestCreate::createHelper(int styleID, int wordID)
{
	int curr = wordID;
	//Tokenizer tok;
	std::vector<int> hold;
	int threshold = 10;
	hold.push_back(curr);
	srand(time(0));

	float random;
	float percent = 0.0;
	int thres = (int)(rand() % 6);
	int max = 0;

	std::vector<WordNextCountStruct>dataW;
	dataW = getNextToken(curr, styleID);
	for (int i = 0; i < dataW.size(); i++)
	{
		max += dataW[i].count;
	}

	int  ind = 0;
	bool lock = false;

	while (!checkPunctuation((char)(curr)) && thres <= threshold)
	{
		random = (float)(rand() % 100) / 100;
		lock = true;
		ind = 0;
		percent = 0.0;
		while (lock)
		{
			if (dataW.size() >= 1 && ind < dataW.size())
			{
				percent += ((float)dataW[ind].count / (float)max);
				if (random <= percent)//&& dataBase.wpd.isWordToken(random, styleID) == true)// Function almost always ret false
				{
					hold.push_back(dataW[ind].wordTokenID);
					hold.push_back((int)('&'));
					curr = dataW[ind].wordTokenID;
					lock = false;
				}
				ind++;
			}
			else
			{
				lock = false;
			}
		}//end of inner loop

		max = 0;
		dataW = getNextToken(curr, styleID);
		for (int i = 0; i < dataW.size(); i++)
		{
			max += dataW[i].count;
		}
		thres++;
	}//end of outer loop

	if (thres > threshold)
	{
		hold.push_back(46); // always a period
	}

	return hold;
}//end of createDocH2()