/*
CPSC 323-02
Names:	Stephen Shinn
Michael Perna
Robert Ruiz

Project: Assignment 1
*/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

struct Token {
	std::string token;
	std::string lexeme;
};

class Parser {
	private:
		//PRINT PRODUCTION RULES
		bool printRules = true;

		std::ifstream input;
		std::ofstream output;

		std::vector<std::string> keywords{ "int", "boolean", "real", "if", "endif", "else", "return", "put", "get", "while", "true", "false", "function" };
		std::vector<std::string> operators{"+", "-", "*", "/", "<", ">", "=", "=>", "=<", "==", "^="};
		std::vector<std::string> separators{";", ":", ",", "{", "}", "(", ")", "[", "]", "%%"};

		int lineNum = 1;

		bool isOpSepChar(char a);
		bool isKeyword(std::string lex);
		bool isOperator(std::string lex);
		bool isSeparator(std::string lex);

		void Parser::printToken(Token x);
		void Parser::match(std::string lexeme);
		void Parser::OFD();
		void Parser::FD();

	public:
		Token testToken;
		Parser::Parser(std::string fileIn, std::string fileOut);
		Token lexer();
		bool iseof();
		int currentLine();
		void Parser::R18S();

};

#endif
