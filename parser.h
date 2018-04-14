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

		//void Parser::printToken(Token x);
		void Parser::printProduction(std::string rule);
		void Parser::errorLex(std::string tok);
		void Parser::errorTok(std::string tok);
		bool Parser::cmpLex(std::string inp);
		bool Parser::cmpTok(std::string inp);
		void Parser::matchLex(std::string lexeme);
		void Parser::matchTok(std::string tok);
		void Parser::OFD();
		void Parser::FD();
		void Parser::FDpr();
		void Parser::F();
		void Parser::OPL();
		void Parser::PL();
		void Parser::PLpr();
		void Parser::P();
		void Parser::Q();
		void Parser::B();
		void Parser::ODL();
		void Parser::DL();
		void Parser::DLpr();
		void Parser::D();
		void Parser::IDS();
		void Parser::IDSpr();
		void Parser::SL();
		void Parser::SLpr();
		void Parser::S();
		void Parser::CMP();
		void Parser::A();
		void Parser::I();
		void Parser::Ipr();
		void Parser::R();
		void Parser::Rpr();
		void Parser::PR();
		void Parser::SC();
		void Parser::W();
		void Parser::CND();
		void Parser::RLP();
		void Parser::E();
		void Parser::Epr();
		void Parser::T();
		void Parser::Tpr();
		void Parser::FA();
		void Parser::PMY();
		void Parser::PMYpr();
		void Parser::EMP();

	public:
		Token testToken;
		Parser::Parser(std::string fileIn, std::string fileOut);
		Token lexer();
		bool iseof();
		int currentLine();
		void Parser::R18S();
		void Parser::printToken(Token x);

};

#endif
