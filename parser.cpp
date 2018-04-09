/*
CPSC 323-02
Names:	Stephen Shinn
Michael Perna
Robert Ruiz

Project: Assignment 1
*/

#include "parser.h"
#include <iomanip>
using namespace std;

Parser::Parser(string fileIn, string fileOut){
	
	input.open(fileIn);
	output.open(fileOut);

	testToken = lexer();
}

/*
========================
IMPLEMENTATION OF LEXER
========================
*/

Token Parser::lexer(){			//lexer function

	string lexeme;	//Strings that will contain data after processing
	string token;
	bool tokenFound = false;

	char testChar;
	testChar = input.get();
	if(testChar == '\n')
		lineNum++;

	while (!tokenFound)
	{
		if (testChar == EOF)
		{
			lexeme.push_back(testChar);
			token = "EOF";
			return Token{token, lexeme};
		}
		else
		if (testChar == '!')
		{
			do {
				testChar = input.get();		//proceed to next character inside comment
				if(testChar == '\n')
					lineNum++;
			} while(testChar != '!');		//iterate through all of comment

			testChar = input.get();			//sets next character to be read
			if(testChar == '\n')
					lineNum++;
		}
		else
		if(isspace(testChar))				//proceed through all whitespace
		{
			do {
				testChar = input.get();
				if(testChar == '\n')
					lineNum++;
			} while(isspace(testChar));
		}
		else
		if (isalpha(testChar))	//FSM for id
		{
			lexeme.push_back(testChar);
			int state = 2;
			int prevState;
			bool validState = true;

			while (validState)
			{
				switch (state)
				{
					case 2:
						testChar = input.get();
						lexeme.push_back(testChar);
						if(isalpha(testChar))
							state = 4;
						else
						if(isdigit(testChar))
							state = 5;
						else
						if(testChar == '$')
							state = 3;
						else
						{
							prevState = state;
							state = 6;
						}
						break;
					case 3:
						testChar = input.get();
						lexeme.push_back(testChar);
						if(isalpha(testChar))
						{
							prevState = state;
							state = 6;
						}
						else
						if(isdigit(testChar))
						{
							prevState = state;
							state = 6;
						}
						else
						if(testChar == '$')
						{
							prevState = state;
							state = 6;
						}
						else
						{
							prevState = state;
							state = 6;
						}
						break;
					case 4:
						testChar = input.get();
						lexeme.push_back(testChar);
						if(isalpha(testChar))
							state = 4;
						else
						if(isdigit(testChar))
							state = 5;
						else
						if(testChar == '$')
							state = 3;
						else
						{
							prevState = state;
							state = 6;
						}
						break;
					case 5:
						testChar = input.get();
						lexeme.push_back(testChar);
						if(isalpha(testChar))
							state = 4;
						else
						if(isdigit(testChar))
							state = 5;
						else
						if(testChar == '$')
							state = 3;
						else
						{
							prevState = state;
							state = 6;
						}
						break;
					case 6:
						input.unget();
						lexeme.pop_back();
						validState = false;
						break;
				}
			}

			switch (prevState)
			{
				case 5:
				case 6:
				default:
					token = "unrecognized";
					break;
				case 2:
				case 3:
					token = "identifier";
					break;
				case 4:
					token = "identifier";
					if (isKeyword(lexeme))
						token = "keyword";
					break;
			}

			tokenFound = true;
		}
		else
		if (isdigit(testChar))	//FSM for int/real
		{
			lexeme.push_back(testChar);
			int state = 2;
			int prevState;
			bool validState = true;

			while (validState)
			{
				switch (state)
				{
					case 2:
						testChar = input.get();
						lexeme.push_back(testChar);
						if(isdigit(testChar))
							state = 2;
						else
						if(testChar == '.')
							state = 3;
						else
						{
							prevState = state;
							state = 5;
						}
						break;
					case 3:
						testChar = input.get();
						lexeme.push_back(testChar);
						if(isdigit(testChar))
							state = 4;
						else
						if(testChar == '.')
						{
							prevState = state;
							state = 5;
						}
						else
						{
							prevState = state;
							state = 5;
						}
						break;
					case 4:
						testChar = input.get();
						lexeme.push_back(testChar);
						if(isdigit(testChar))
							state = 4;
						else
						if(testChar == '.')
						{
							prevState = state;
							state = 5;
						}
						else
						{
							prevState = state;
							state = 5;
						}
						break;
					case 5:
						input.unget();
						lexeme.pop_back();
						validState = false;
						break;
				}
			}

			switch (prevState)
			{
				case 2:
					token = "integer";
					break;
				case 4:
					token = "real";
					break;
				case 3:
				case 5:
					token = "unrecognized";
					break;
			}

			tokenFound = true;
		}
		else
		if (isOpSepChar(testChar))	//Check for op/sep
		{
			lexeme.push_back(testChar);
			bool opSepFound = false;

			while (!opSepFound)
			{
				char peek;

				switch (testChar)
				{
					case '+':
					case '-':
					case '*':
					case '/':
					case '<':
					case '>':
					case ';':
					case ':':
					case ',':
					case '{':
					case '}':
					case '(':
					case ')':
					case '[':
					case ']':
						opSepFound = true;
						break;
					//Special Case Characters
					case '=':
						peek = input.peek();
						if ((peek == '>') || (peek == '<') || (peek == '='))
						{
							testChar = input.get();
							lexeme.push_back(testChar);
						}
						opSepFound = true;
						break;
					case '^':
						peek = input.peek();
						if (peek == '=')
						{
							testChar = input.get();
							lexeme.push_back(testChar);
						}
						opSepFound = true;
						break;
					case '%':
						peek = input.peek();
						if (peek == '%')
						{
							testChar = input.get();
							lexeme.push_back(testChar);
						}
						opSepFound = true;
						break;
				}
			}

			if(isOperator(lexeme))
				token = "operator";
			else
			if(isSeparator(lexeme))
				token = "separator";
			else
				token = "unrecognized";

			tokenFound = true;
		}
		else
		{	
			lexeme.push_back(testChar);
			token = "unrecognized";
			tokenFound = true;
		}
	}
	return Token{token, lexeme};
}

bool Parser::isOpSepChar(char a)
{
	switch (a)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '<':
		case '>':
		case ';':
		case ':':
		case ',':
		case '{':
		case '}':
		case '(':
		case ')':
		case '[':
		case ']':
		//Special Case Characters
		case '=':
		case '^':
		case '%': 
			return true;
		default:
			break;
	}

	return false;
}

bool Parser::isKeyword(string lex)
{
	for (auto x : keywords)
		if (lex.compare(x) == 0)
			return true;
	return false;
}

bool Parser::isOperator(string lex)
{
	for (auto x : operators)
		if(lex.compare(x) == 0)
			return true;
	return false;
}

bool Parser::isSeparator(string lex)
{
	for(auto x : separators)
		if(lex.compare(x) == 0)
			return true;
	return false;
}

bool Parser::iseof()
{
	if (input.peek() == EOF)
		return true;
	else
		return false;
}

int Parser::currentLine()
{
	return lineNum;
}

/*
==================================
IMPLEMENTATION OF SYNTAX ANALYZER
==================================
*/
void Parser::printToken(Token x)
{
	cout << "Token: " << setw(15) << left  << x.token << "Lexeme: " << setw(15) << left << x.lexeme << endl;
	output << "Token: " << setw(15) << left  << x.token << "Lexeme: " << setw(15) << left << x.lexeme << endl;
}

void Parser::match(std::string lexeme)
{
	if (testToken.lexeme.compare(lexeme) == 0)
	{
		testToken = lexer();
	}
	else
		cout << "Error: Expected " << lexeme << " in line " << lineNum;
}


void Parser::R18S() {

	if (printRules)
	{
		printToken(testToken);
		cout << "\t<Rat18S> ::= <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" << endl;
		output << "\t<Rat18S> ::= <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" << endl;
	}

	OFD();
	
	if (testToken.lexeme.compare("%%") == 0)
	{
		match("%%");
		//ODL();
		//SL();
	}
	else
	{
		cout << "Error: Expected %% in line " << lineNum;
		output << "Error: Expected %% in line " << lineNum;
	}
		

};

void Parser::OFD() {

	if (printRules)
	{
		cout << "\t<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl;
		output << "\t<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl;
	}

	FD();
}

void Parser::FD() {
	
	if (printRules)
	{
		cout << "\t<Function Definitions> ::= <Function> | <Function> <Function Definitions>" << endl;
		output << "\t<Function Definitions> ::= <Function> | <Function> <Function Definitions>" << endl;
	}



}