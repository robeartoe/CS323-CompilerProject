/*
CPSC 323-02
Names:	Stephen Shinn
Michael Perna
Robert Ruiz

Project: Assignment 1
*/

#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

struct Token {
	std::string token;
	std::string lexeme;
};

class Scanner {
	private:
		std::ifstream input;
		std::vector<std::string> keywords{ "int", "boolean", "real", "if", "endif", "else", "return", "put", "get", "while", "true", "false" };
		bool isUnrecognized(char a);
		bool isOpSepChar(char a);
		bool isSpecNextChar(char a, char b);
		bool isSpecChar(char a, char b);
		bool isKeyword(std::string tkn);
		Token intRealFSM(std::string tkn);
		Token idFSM(std::string tkn);
		Token opSepCheck(std::string tkn);

	public:
		Scanner(std::string fileName);
		Token lexer();
		bool iseof();
};

#endif
