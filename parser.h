/*
CPSC 323-02
Names:	Stephen Shinn
Michael Perna
Robert Ruiz

Project: Assignment 3
*/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "tables.h"

struct Token {
  std::string token;
  std::string lexeme;
};

class Parser {
	private:
		//PRINT PRODUCTION RULES
		bool printRules = true;

        SymbolTable sym_table_;
		InstructionTable inst_table_;

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

		//void printToken(Token x);
		void printProduction(std::string rule);
		void errorLex(std::string tok);
		void errorTok(std::string tok);
		bool cmpLex(std::string inp);
		bool cmpTok(std::string inp);
		void matchLex(std::string lexeme);
		void matchTok(std::string tok);
		void OFD();
		void FD();
		void FDpr();
		void F();
		void OPL();
		void PL();
		void PLpr();
		void P();
		void Q();
		void B();
		void ODL();
		void DL();
		void DLpr();
		void D();
		void IDS();
		void IDSpr();
		void SL();
		void SLpr();
		void S();
		void CMP();
		void A();
		void I();
		void Ipr();
		void R();
		void Rpr();
		void PR();
		void SC();
		void W();
		void CND();
		void RLP();
		void E();
		void Epr();
		void T();
		void Tpr();
		void FA();
		void PMY();
		void PMYpr();
		void EMP();
       
	public:
		Token testToken;
		Parser(std::string fileIn, std::string fileOut);
		Token lexer();
		bool iseof();
		int currentLine();
		void R18S();
		void printToken(Token x);
		void parseMsg();
		void print_tables();

};

#endif
