
#include <sstream>
#include "HTMLgenerator.h"
#include "Database.h"
#include <fstream>
#include <iostream>


HTMLgenerator::HTMLgenerator(string DestinationFilename, int documentID, vector<SentenceRanking> rankingList)
{
	// Blake, this is one of the harder challenges in the program.  Since it is sometimes hard to see what a designer is 
	// envisioning, I wanted to give a basic set of sample code for how to access the database

	//StyleDatabase& db = StyleDatabase::getInstance();
	//db.open("aisql.db3");
	StyleDatabase db;
	db.open();
	string sentence; // the current sentence the function is working with
	stringstream output; // the output that will be saved to the DestinationFilename
	

	//Basically I just want to take the contents of the header.txt file and put it in the output stream here
	// If you can do it better, please do.
	ifstream infile;
	string header;
	infile.open("html/header.txt");
	infile >> header;
	output << header;
	infile.close();

	if (rankingList.size()!=0)
	{
		//Output how much your document was plagarised
		// ********I don't know how to call this, any ideas? insert between the bold tags(There is no input for this either)
		output << "<p>Your document was plagiarized <b>" << "</b></p>";

		for (std::vector<SentenceRanking>::size_type i = 0; i != rankingList.size(); i++)
		{
			//header
			output << "<div id='doc-wrapper'>";
			output << "<div id='left-column'><h3>";
			//insert left header title
			output << "</h3></div>";
			output << "<div id='right-column'><h3>";
			//insert right header title
			output << "</h3></div>";
			output << "</div>";


			//Generate the left column first (That's the document that the user gave us)
			//output << "<div id='doc-wrapper'><div id='left-column'>";
			//output << db.getSentence(rankingList[i].foundSentenceID - 2) << "  ";
			//output << db.getSentence(rankingList[i].foundSentenceID - 1) << "  ";
			//output << "<div id='popup' style='background-color: yellow;'>";
			//output << db.getSentence(rankingList[i].foundSentenceID) << "  ";
			////add spans for probability
			//output << "<span>This section was plagiarized: <b>" << rankingList[i].certainty << "</b></span></div>";
			//output << db.getSentence(rankingList[i].foundSentenceID + 1) << "  ";
			//output << db.getSentence(rankingList[i].foundSentenceID + 2) << "  ";
			output << "</div'>"; //end left-column


			output << "<div id='right-column'>"; //begin right column
			//output << db.getSentence(rankingList[i].foundSentenceID - 2) << "  ";
			//output << db.getSentence(rankingList[i].foundSentenceID - 1) << "  ";
			//output << "<div id='popup' style='background-color: yellow;'>";
			//output << db.getSentence(rankingList[i].foundSentenceID) << "  ";
			//output << "<span>This section was plagiarized: <b>" << rankingList[i].certainty << "</b></span></div>";
			//output << db.getSentence(rankingList[i].foundSentenceID + 1) << "  ";
			//output << db.getSentence(rankingList[i].foundSentenceID + 2) << "  ";

			output << "</div>"; //end the doc-wrapper
		}
	}
	else
	{
		output << "<p>Your document was not plagiarized!</p>";
	}

	//Basically I just want to take the contents of the footer.txt file and put it in the output stream here
	// If you can do it better, please do.
	//ifstream infile;
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
	db.close();

}


HTMLgenerator::~HTMLgenerator()
{
}
