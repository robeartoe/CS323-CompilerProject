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
		std::vector<std::string> keywords{ "int", "boolean", "real", "if", "endif", "else", "return", "put", "get", "while", "true", "false", "function" };
		std::vector<std::string> operators{"+", "-", "*", "/", "<", ">", "=", "=>", "=<", "==", "^="};
		std::vector<std::string> separators{";", ":", ",", "{", "}", "(", ")", "[", "]", "%%"};
		bool Scanner::isOpSepChar(char a);
		bool Scanner::isKeyword(std::string lex);
		bool Scanner::isOperator(std::string lex);
		bool Scanner::isSeparator(std::string lex);

	public:
		Scanner::Scanner(std::string fileName);
		Token Scanner::lexer();
		bool Scanner::iseof();
};

#endif
