/*
//  --------------PROJECT DESCRIPTION-------------  //

Program 1: DNA Profiling
Course: CS 251, Spring 2022. Wed 12pm lab
System: Windows OS using Repl.it
Author: Nandini Jirobe

About the program:
  This is the DNA Profling App. This program builds DNA strands using the
provided ourvector implementation and then determines who the DNA matches to in
given a database

// ---------ABOUT MILESTONE #6: CREATIVE COMPONENT ------  //
About Milestone #6: Creative Component.
  The command "add_person" has been added to the app. This allows the user to
add people to database which are currently in use. A database must be loaded
before using this function!

Instructions:
  1) When prompted, input the name of the person.
  2) Then input the data of each STR type asked.

  The program will let you know when the person has been added to the database
currently in use.

Below is an example of how add_person works:

  Enter command or # to exit: load_db small.txt
  Loading database...
  Enter command or # to exit: add_person
  Please enter the person's name: Goofy
  AGATC: 4
  AATG: 7
  TATC: 5
  Goofy successfully added!

------------------------------------------------
*/

#include <iostream>

#include <fstream>

#include "ourvector.h"

using namespace std;

//  --------------STRUCTS--------------------  //
//  Stores a person name and their DNA's STR values
struct PersonDNAInfo {
	string name;
	ourvector<int> numSTR;
};

//  -----------HELPER FUNCTIONS------------  //

/*
Parameters:
  fileName which is string
  fileData which is a vector of strings

Return type:
  Integer

Purpose:
  This function reads a file line by line and converts it into a string.
  Each string is then added the fileData vector.
  If the file reading was successful, the function returns a 1.
  If the file reading fails, the funciton returns a 0

*/
int readFileToStringVector(string fileName, ourvector<string> &fileData) {
	ifstream inFS(fileName);
	string lineValue;

	if (!inFS.is_open()) {
		cout << "Error: unable to open '" << fileName << "'" << endl;
		return 0;
	} else {
		while (!inFS.eof()) {
			getline(inFS, lineValue, '\n');
			if (!inFS.fail()) {
				fileData.push_back(lineValue);
			}
		} //  end while loop
	}

	return 1;
} //  end readFileToStringVector();

/*
Parameters:
  fileName which is string
  fileData which is a vector of chars

Return type:
  Integer

Purpose:
  This function reads a file, and then adds each character in the
  file into the fileData vector.
  If the file reading was successful, the function returns a 1.
  If the file reading fails, the funciton returns a 0

*/
int readFileToCharVector(string fileName, ourvector<char> &fileData) {
	ifstream inFS(fileName);
	char letter;

	if (!inFS.is_open()) {
		cout << "Error: unable to open '" << fileName << "'" << endl;
		return 0;
	} else {
		while (!inFS.eof()) {
			inFS >> letter;
			if (!inFS.fail()) {
				fileData.push_back(letter);
			}
		} //  end while loop
	}

	return 1;
} //  end readFileToCharVector();

/*
Parameters:
  theString is a string type
  storeInHere is a vector of

Return type:
  No return value

Purpose:
  This function takes a string of numbers and splits
  wherever is sees a ',' character. The numbers are
  then added to the storeInHere vector
*/
void stringSplitter(string theString, ourvector<int> &storeInHere) {
	ourvector<int> wordSplitIndexes;
	wordSplitIndexes.push_back(-1);
	//  for loop goes through the string and stores the
	//  the starting and ending indexes of each number present
	//  in the string.
	for (int i = 0; i < theString.size(); i++) {
		if (theString.at(i) == ',') {
			wordSplitIndexes.push_back(i);
		}
	}
	wordSplitIndexes.push_back(theString.size());

	//  Find each number, convert to integer and add it
	//  to the storeInHere vector
	for (int i = 0; i < wordSplitIndexes.size() - 1; i++) {
		int numberStart = wordSplitIndexes.at(i) + 1;
		int numberEnd =
			wordSplitIndexes.at(i + 1) - (wordSplitIndexes.at(i) + 1);
		storeInHere.push_back(stoi(theString.substr(numberStart, numberEnd)));
	}
} //  end stringSplitter()

/*
Parameters:
  integer type vector

Return type:
  No return value

Purpose:
  This function prints out the integers of a given vector
*/
void printIntVector(ourvector<int> someVector) {
	for (int i = 0; i < someVector.size(); i++) {
		cout << " " << someVector.at(i);
	}
} //  end printIntVector()

/*
Parameters:
  char type vector

Return type:
  No return value

Purpose:
  This function prints out the characters of a given vector
*/
void printCharVector(ourvector<char> someVector) {
	for (int i = 0; i < someVector.size(); i++) {
		cout << someVector.at(i);
	}
} //  end printCharVector()

/*
Parameters:
  strType is char vector
  dnaSample is a char vector

Return type:
  integer

Purpose:
  This function returns the maximum number of consecutive
  occurances of an STR type (from strType vector) present
  in a DNA Sample (from dnaSample vector)
*/
int countNumOccurances(ourvector<char> strType, ourvector<char> dnaSample) {
	int consecOccurances = 0;
	int maxConsecOccurances = 0;
	//  Loop though DNA sample
	for (int i = 0; i < dnaSample.size(); i++) {
		int numSame = 0;
		//  Loop though the STR type
		for (int j = 0; j < strType.size(); j++) {
			//  Increment numSame if a comman letter is found.
			if (i + j < dnaSample.size()) {
				if (dnaSample.at(i + j) == strType.at(j)) {
					numSame++;
				} else {
					break;
				}
			}
		} //  end inner for loop

		//  Increment consecOccurances if exact match is found
		if (numSame == strType.size()) {
			consecOccurances++;
			i += (strType.size() - 1);
		} else {
			//  Reset consecOccurances to 0
			//  update maxConsecOccurances if necessary
			if (consecOccurances >= maxConsecOccurances) {
				maxConsecOccurances = consecOccurances;
			}
			consecOccurances = 0;
		}
	} //  end outer for loop

	//  Update maxConsecOccurances if necessary
	if (consecOccurances >= maxConsecOccurances) {
		maxConsecOccurances = consecOccurances;
	}

	return maxConsecOccurances;
} //  end countNumOccurances()

/*
Parameters:
  fileData is a string vector type
  peopleDNA is a vector of PersonDNAInfo struct objects
  strTypes is a vector of char vectors.

Return type:
  No return value

Purpose:
  The database text files are structures like a table.
  This function stores the column information (excluding the name)
  in the vector strTypes. Peoples' names and STR counts are stored
  seperately in a struct which is then added to peopleDNA vector
*/
void organizeData(
	ourvector<string> fileData,
	ourvector<PersonDNAInfo> &peopleDNA,
	ourvector<ourvector<char>> &strTypes) {
	//  ---STORE COLUMN INFO-----  //
	string columnInfo = fileData.at(0);
	int index = 5; //  index starts at 5 so we don't include "name,"
	ourvector<char> oneStrType;
	for (int i = 5; i < columnInfo.size(); i++) {
		if (columnInfo.at(i) != ',') {
			oneStrType.push_back(columnInfo.at(i));
		}
		if (columnInfo.at(i) == ',' || i == columnInfo.length() - 1) {
			strTypes.push_back(oneStrType);
			oneStrType.clear();
		}
	}
	//  ---STORE PEOPLE INFO---  //
	for (int i = 1; i < fileData.size(); i++) {
		PersonDNAInfo tempPerson;
		string lineOfInfo = fileData.at(i);
		tempPerson.name = lineOfInfo.substr(0, lineOfInfo.find(','));
		stringSplitter(
			lineOfInfo.substr(lineOfInfo.find(',') + 1, lineOfInfo.length()),
			tempPerson.numSTR);
		peopleDNA.push_back(tempPerson);
	}
} //  end organizeData()

/*
Parameters:
  peopleDNAInfo which is vector of PersonDNAInfo struct objects
  strCounts is a vector of integers
  personName is a string type

Return type:
  Integer

Purpose:
  This function matches matches a DNA sample to a person.
  It returns 1 if the person is found
  It returns 0 is the person is not found

*/
int searchDataBase(
	ourvector<PersonDNAInfo> peopleDNA,
	ourvector<int> strCounts,
	string &personName) {
	//  loops through each persons DNA STR info
	for (int i = 0; i < peopleDNA.size(); i++) {
		int count = 0;
		personName = (peopleDNA.at(i)).name;
		ourvector<int> dnaInfo = (peopleDNA.at(i)).numSTR;

		//  person's DNA info is compared to the DNA Sample
		for (int j = 0; j < dnaInfo.size(); j++) {
			if (dnaInfo.at(j) == strCounts.at(j)) {
				count++;
			}
			if (count == strCounts.size()) {
				return 1;
			}
		} //  end inner for loop
	}
	return 0;
} //  end searchDataBase()

//  --------------MENU FUNCTIONS------------  //

/*
Parameters:
  fileData is string type vector
  strTypes is a char type vector
  peopleDNA is a PersonDNAInfo struct object vector
  databaseLoaded is a bool variable
Return type:
  No return value
Purpose:
  This is a menu function which loads in a database textfile. The information
from the textfile is used to update the strTypes and peopleDNA. databaseLoaded
will be set true if the textfile is successfully loaded.
*/
void load_db(
	ourvector<string> fileData,
	ourvector<ourvector<char>> &strTypes,
	ourvector<PersonDNAInfo> &peopleDNA,
	bool &databaseLoaded) {
	string fileName = "";
	//  Any previous data stored should be cleared. Restart
	fileData.clear();
	strTypes.clear();
	peopleDNA.clear();
	databaseLoaded = false;

	//  take in user input file name and read in data
	cin >> fileName;
	cout << "Loading database..." << endl;
	int readSuccess = readFileToStringVector(fileName, fileData);
	if (readSuccess) {
		//  organize data
		organizeData(fileData, peopleDNA, strTypes);
		databaseLoaded = true;
	}
} //  end load_db()

/*
Parameters:
  fileData is a string type vector
  dnaSample is a char type vector
  dnaLoaded is a bool variable
Return type:
  No return type
Purpose:
  This is a menu function which loads in a DNA sample textfile and converts it
into a vector. Once finished it will set dnaLoaded to true to indicate the DNA
has been successfully loaded
*/
void load_dna(
	ourvector<string> fileData,
	ourvector<char> &dnaSample,
	bool &dnaLoaded) {
	string fileName = "";

	//  clear and previoulsy stored data
	dnaSample.clear();
	fileData.clear();

	//  take in user input filename and read in data
	cin >> fileName;
	cout << "Loading DNA..." << endl;
	int readSuccess = readFileToCharVector(fileName, dnaSample);
	if (readSuccess) {
		dnaLoaded = true;
	}

} //  end load_dna()
/*
Parameters:
  strCounts is a integer type vector
  strTypes is a vector of char type vectors
  dnaSample is a char type vector
  dnaProcessed is a bool variable

Return type:
  No return type
Purpose:
  This function goes through the DNA sample and finds the highest number of
consecutive occurances for each STR type. The information is then updated to
strCounts and dnaProcessed will be set to true if the processing was successful.
*/
void process(
	ourvector<int> &strCounts,
	ourvector<ourvector<char>> strTypes,
	ourvector<char> dnaSample,
	bool &dnaProcessed) {
	//  clear and previosly stored data
	strCounts.clear();
	//  Actually find the str occurances in the data file
	cout << "Processing DNA..." << endl;
	for (int i = 0; i < strTypes.size(); i++) {
		int strCount = countNumOccurances(strTypes.at(i), dnaSample);
		strCounts.push_back(strCount);
	}
	dnaProcessed = true;
} //  end process()

/*
Parameters:
  peopleDNA is a vector of PersonDNAInfo struct objects
  strCounts is a vector of integers
  databaseLoaded, dnaLoaded, and dnaProcessed are all bool variables
Return type:
  No return type
Purpose:
  This function compares the DNA sample's STR counts with the STR counts with
the people in the database. The program will then output information to let the
user know if a match was found.
*/
void search(
	ourvector<PersonDNAInfo> peopleDNA,
	ourvector<int> strCounts,
	bool databaseLoaded,
	bool dnaLoaded,
	bool dnaProcessed) {
	if (!databaseLoaded) {
		cout << "No database loaded." << endl;
	} else if (!dnaLoaded) {
		cout << "No DNA loaded." << endl;
	} else if (!dnaProcessed) {
		cout << "No DNA processed." << endl;
	} else {
		//  search person with DNA match
		string personMatched = "";
		cout << "Searching database..." << endl;
		bool found = searchDataBase(peopleDNA, strCounts, personMatched);
		if (found) {
			cout << "Found in database!  DNA matches: " << personMatched
				 << endl;
		} else {
			cout << "Not found in database." << endl;
		}
	}
} //  end search()

/*
Parameters:
  peopleDNA is a vector of PersonDNAInfo struct objects
Return type:
  No return type
Purpose:
  This function displays the each person in the database and their STR counts
*/
void displayDatabase(ourvector<PersonDNAInfo> peopleDNA) {
	cout << "Database loaded: " << endl;
	for (int i = 0; i < peopleDNA.size(); i++) {
		cout << peopleDNA.at(i).name;
		printIntVector(peopleDNA.at(i).numSTR);
		cout << endl;
	}
	cout << endl;
} //  end displayDatabase()

/*
Parameters:
  dnaSample is a char type vector
Return type:
  No return type
Purpose:
  This function displays the each person in the database and their STR counts
*/
void displayDNA(ourvector<char> dnaSample) {
	cout << "DNA loaded: " << endl;
	for (int i = 0; i < dnaSample.size(); i++) {
		cout << dnaSample.at(i);
	}
	cout << endl << endl;
}

/*
Parameters:
  dnaSample is a char type vector
  strCounts is a int type vector
Return type:
  No return type
Purpose:
  This function displays the STR counts from the DNA sample
*/
void displaySTRCounts(
	ourvector<int> strCounts,
	ourvector<ourvector<char>> strTypes) {
	cout << "DNA processed, STR counts: " << endl;
	for (int i = 0; i < strCounts.size(); i++) {
		printCharVector(strTypes.at(i));
		cout << ": " << strCounts.at(i) << endl;
	}
	cout << endl;
} //  end displaySTRCounts()

/*
Parameters:
  strTypes is a vector of char type vectors
  peopleDNA is a vector of PersonDNAInfo struct objects
Return type:
  No return type
Purpose:
   This function allows uses to add people to the database which is currently is
use.
*/
void addPerson(
	ourvector<ourvector<char>> strTypes,
	ourvector<PersonDNAInfo> &peopleDNA) {
	PersonDNAInfo somePerson;
	int number;
	//  take in person's name
	cout << "Please enter the person's name: ";
	cin >> somePerson.name;
	//  take in person's str data
	for (int i = 0; i < strTypes.size(); i++) {
		printCharVector(strTypes.at(i));
		cout << ": ";
		cin >> number;
		(somePerson.numSTR).push_back(number);
	}
	peopleDNA.push_back(somePerson);
	cout << somePerson.name << " successfully added!" << endl;
}

int main() {
	//  declare variables
	string fileName = "";
	string userCommand = "";
	ourvector<int> strCounts;
	ourvector<char> dnaSample;
	ourvector<string> fileData;
	ourvector<PersonDNAInfo> peopleDNA;
	ourvector<ourvector<char>> strTypes;
	bool dnaLoaded = false;
	bool dnaProcessed = false;
	bool databaseLoaded = false;

	cout << "Welcome to the DNA Profiling Application." << endl;

	while (userCommand != "#") {
		cout << "Enter command or # to exit: ";
		cin >> userCommand;

		if (userCommand == "load_db") {
			load_db(fileData, strTypes, peopleDNA, databaseLoaded);
		} else if (userCommand == "load_dna") {
			load_dna(fileData, dnaSample, dnaLoaded);
		} else if (userCommand == "display") {
			//  check if the datase is loaded. Display accordingly
			//  This displays the people in the db with their dna info
			if (databaseLoaded) {
				displayDatabase(peopleDNA);
			} else {
				cout << "No database loaded." << endl;
			}
			//  check if DNA is laoded. Display accordingly
			//  This display the dna sample we are trying to match
			if (dnaLoaded) {
				displayDNA(dnaSample);
			} else {
				cout << "No DNA loaded." << endl << endl;
			}
			//  check if dna has been processed. Display accordingly
			//  This displays the number of each STR count in the dna sample
			if (dnaProcessed) {
				displaySTRCounts(strCounts, strTypes);
			} else {
				cout << "No DNA has been processed." << endl;
			}
		} else if (userCommand == "process") {
			//  make sure you have all you need before you process the data
			if (!databaseLoaded) {
				cout << "No database loaded." << endl;
			} else if (!dnaLoaded) {
				cout << "No DNA loaded." << endl;
			} else {
				process(strCounts, strTypes, dnaSample, dnaProcessed);
			}
		} else if (userCommand == "search") {
			search(
				peopleDNA, strCounts, databaseLoaded, dnaLoaded, dnaProcessed);
		} else if (userCommand == "add_person") {
			if (databaseLoaded) {
				addPerson(strTypes, peopleDNA);
			} else {
				cout << "No database loaded." << endl;
			}
		}
	} //  end while loop
	return 0;
} //  end main()
