//Main File to read file:

/*
The first assignment is to write a lexical analyzer (lexer)
You can build your entire lexer using a FSM, Or build using at least FSMs for identifier, integer and real (the rest can be written ad-hoc)
but YOU HAVE TO CONSTRUCT A FSM for this assignment otherwise, there will be a deduction of 2 points!
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <streambuf>
#include <vector>

#include "scanner.h"
using namespace std;

int main(){

	string fileInputName,fileOutputName;	// File Handles to be Processed
	vector<Token> tkns;

	std::cout << "Input Name of File: ";
	getline(cin,fileInputName);

	//std::cout << "Input Name of Output File: ";
	//getline(cin,fileOutputName);

	Scanner lex = Scanner::Scanner(fileInputName);

	while (!lex.iseof())
		tkns.push_back(lex.lexer());


	for (int i = 0; i < tkns.size(); i++)
	{
		cout << "token: " << tkns[i].token << "\tlexeme: " << tkns[i].lexeme << endl << endl;
	}

	return 0;
}