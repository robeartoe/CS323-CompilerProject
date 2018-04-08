/*
CPSC 323-02
Names:	Stephen Shinn
		Michael Perna
		Robert Ruiz

Project: Assignment 1
*/

#include "scanner.h"
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

	ofstream output;
	output.open(fileOutputName.c_str());

	Scanner lex(fileInputName);


	cout << setw(20) << left << "Token" << setw(20) << "Lexeme" << endl;
	output << setw(20) << left << "Token" << setw(20) << "Lexeme" << endl;
	cout << setw(20) << left << string(5, '=') << setw(20) << string(6, '=') << endl;
	output << setw(20) << left << string(5, '=') << setw(20) << string(6, '=') << endl;
	while (!lex.iseof())
	{
		tkns = lex.lexer();
		if (tkns.lexeme.at(0) != EOF)
		{
			cout << setw(20) << tkns.token << setw(20) << left << tkns.lexeme << endl;
			output << setw(20) << tkns.token << setw(20) << left << tkns.lexeme << endl;
		}
	}

	return 0;
}