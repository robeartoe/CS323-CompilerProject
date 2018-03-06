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
		bool Scanner::isUnrecognized(char a);
		bool Scanner::isOpSepChar(char a);
		bool Scanner::isSpecNextChar(char a, char b);
		bool Scanner::isSpecChar(char a, char b);
		bool Scanner::isKeyword(std::string tkn);
		Token Scanner::intRealFSM(std::string tkn);
		Token Scanner::idFSM(std::string tkn);
		Token Scanner::opSepCheck(std::string tkn);

	public:
		Scanner::Scanner(std::string fileName);
		Token Scanner::lexer();
		bool Scanner::iseof();
};

#endif
