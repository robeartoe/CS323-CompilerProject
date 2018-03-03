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
		bool Scanner::isOpSepChar(char a);
		bool Scanner::isSpecNextChar(char a);
		bool Scanner::lookAhead(char a);
		Token Scanner::intRealFSM(std::string tkn);
		Token Scanner::idFSM(std::string tkn);

	public:
		Scanner::Scanner(std::string fileName);
		Token Scanner::lexer();
		bool Scanner::iseof();
};

#endif
