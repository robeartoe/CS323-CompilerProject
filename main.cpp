/*
CPSC 323-02
Names:	Stephen Shinn
		Michael Perna
		Robert Ruiz

Project: Assignment 1
*/

#include "scanner.h"
using namespace std;

bool fileExists(std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

int main(){

	string fileInputName,fileOutputName;	// File Handles to be Processed
	Token tkns;


	std::cout << "Please enter the name of the input file: ";
	getline(cin, fileInputName);

	if (!fileExists(fileInputName))
	{
		cout << "File does not exist!";
		exit(0);
	}

	cout << endl;

	
	std::cout << "Please enter the name of the output file: ";
	getline(cin,fileOutputName);

	ofstream output;
	output.open(fileOutputName);

	Scanner lex = Scanner::Scanner(fileInputName);


	cout << "Token\t\t\tLexeme" << endl;
	output << "Token\t\t\tLexeme" << endl;
	while (!lex.iseof())
	{
		tkns = lex.lexer();
		if (tkns.token.at(0) != EOF)
		{
			cout << "token: " << tkns.token << "\t\t\tlexeme: " << tkns.lexeme << endl;
			output << "token: " << tkns.token << "\t\t\tlexeme: " << tkns.lexeme << endl;
		}
	}

	return 0;
}