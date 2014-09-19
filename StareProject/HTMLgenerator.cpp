
#include <sstream>
#include "HTMLgenerator.h"
#include "Database.h"
#include <fstream>
#include <iostream>


HTMLgenerator::HTMLgenerator(string DestinationFilename, int documentID, vector<sentenceRanking> rankingList)
{
	// Blake, this is one of the harder challenges in the program.  Since it is sometimes hard to see what a designer is 
	// envisioning, I wanted to give a basic set of sample code for how to access the database

	StyleDatabase db;
	string sentence; // the current sentence the function is working with
	stringstream output; // the output that will be saved to the DestinationFilename
	//My code  "***" means there needs to be an insertion here. 

	//Basically I just want to take the contents of the header.txt file and put it in the output stream here
	// If you can do it better, please do.
	ifstream infile;
	string header;
	infile.open("html/header.txt");
	infile >> header;
	output << header;
	infile.close();
	//Output how much your document was plagarised
	// ********I don't know how to call this, any ideas? insert between the bold tags(There is no input for this either)
	output << "<p>Your document was plagiarized <b>" << "</b></p>";



	//Generate the left column first (That's the document that the user gave us)
	for (std::vector<sentenceRanking>::size_type i = 0; i != rankingList.size(); i++)
	{
		output << "<div id='doc-wrapper'><div id='left-column'>";
		//**** Output your document in context.  (+-2 sentences before and after)
		//rankingList[i].sentenceID-2 this needs the actual sentence lookup
		//rankingList[i].sentenceID-1 this needs the actual sentence lookup
		output << "<div id='popup' style='background-color: yellow;'>";
		//rankingList[i].sentenceID this needs the actual sentence lookup
		//add spans for probability
		output << "<span>This section was plagiarized: <b>" << "</b></span></div>";
		//rankingList[i].sentenceID+1 this needs the actual sentence lookup
		//rankingList[i].sentenceID+2 this needs the actual sentence lookup
		output << "</div'>"; //end left-column


		output << "<div id='right-column'>"; //begin right column
		//*** I have the current found sentence, I need to look up this sentence in the DB then I also want the 
		// 2 sentences before and after, so "rankingList[i].foundSentenceID+2",  "rankingList[i].foundSentenceID+2"
		// after lookups
		//rankingList[i].foundSentenceID;

		//******current sentence want to potentially add different colors...later
		output << "<div id='popup' style='background-color: yellow;'>";
		//******after the sentence add spans for the popup need probability
		output << "<span>This section was plagiarized: <b>" << "</b></span></div>";
		//****INSERT next 2 sentences

		output << "</div>"; //end the doc-wrapper
	}

	//Basically I just want to take the contents of the footer.txt file and put it in the output stream here
	// If you can do it better, please do.
	ifstream infile;
	string footer;
	infile.open("html/footer.txt");
	infile >> footer;
	output << footer;
	infile.close();

	//output the file, I think this is right?
	std::filebuf fb;
	fb.open(DestinationFilename, std::ios::out);
	std::ostream outStream(&fb);
	outStream << output.str();
	fb.close();





	//Brians Code

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
