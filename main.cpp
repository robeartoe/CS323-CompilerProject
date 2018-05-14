/*
CPSC 323-02
Names:	Stephen Shinn
		Michael Perna
		Robert Ruiz

Project: Assignment 3
*/

#include "parser.h"
#include "tables.h"
#include <iomanip>
#include <cstdlib>

using namespace std;

bool fileExists(std::string fileName)
{
	std::ifstream infile(fileName.c_str());
	return infile.good();
}

int main(){

	string fileInputName,fileOutputName;	// File Handles to be Processed
	Token tkns;


	std::cout << "Please enter the name of the input file: ";
	getline(cin, fileInputName);

	cout << endl;

	if (!fileExists(fileInputName))
	{
		cout << "File does not exist!" << endl << endl;
		system("pause");
		exit(0);
	}


	std::cout << "Please enter the name of the output file: ";
	getline(cin,fileOutputName);

	cout << endl;


	Parser synckr(fileInputName, fileOutputName);

	synckr.R18S();

	//If last token is EOF, parsing successful
	synckr.parseMsg();
    synckr.print_tables();

	return 0;
}
