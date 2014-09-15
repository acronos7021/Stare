
#include <sstream>
#include "HTMLgenerator.h"
#include "Database.h"


HTMLgenerator::HTMLgenerator(string DestinationFilename, int documentID, vector<sentenceRanking> rankingList)
{
	// Blake, this is one of the harder challenges in the program.  Since it is sometimes hard to see what a designer is 
	// envisioning, I wanted to give a basic set of sample code for how to access the database

	StyleDatabase db;
	string sentence; // the current sentence the function is working with
	stringstream output; // the output that will be saved to the DestinationFilename

	// nextRank allows comparison of the next rankingList sentence with the next sentence pulled from the database
	// if there is a match, you would change the HTML color and add an to reference the similarities.
	vector<sentenceRanking>::iterator nextRank = rankingList.begin();

	int sentenceID = 0;
	while ((sentence = db.getSentence(sentenceID)) != "")
	{
		if (sentenceID == nextRank->sentenceID)
		{
			// this is a plagerized sentence so do fancy formatting.

			// setup HTML code that is related to a highlighted html sentence
			output << sentence;
			// finish highlighting HTML code.
			
			// If nextRank is at the end, just leave it there.  It will not need to match any future sentences.
			// otherwise move to the next rankingList item since we've already handled this one.
			if (nextRank != rankingList.end())
				nextRank++;
		}
		else
		{
			// this is a normal sentence so it doesn't need any special formatting.
			output << sentence;
		}
		sentenceID++;
	}
	// write output to file.
}


HTMLgenerator::~HTMLgenerator()
{
}
