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
		printToken(testToken);
	}
	else
		cout << "Error: Expected " << lexeme << " in line " << lineNum;
}

void Parser::matchType(std::string tok)
{
	if (testToken.token.compare(tok) == 0)
	{
		testToken = lexer();
		printToken(testToken);
	}
	else
		cout << "Error: Expected a(n) " << tok << " in line " << lineNum;
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
		ODL();
		SL();
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

	if (testToken.lexeme.compare("function") == 0)
	{
		FD();
	}
	else
		return;
}

void Parser::FD() {
	
	if (printRules)
	{
		cout << "\t<Function Definitions> ::= <Function> | <Function> <Function Definitions>" << endl;
		output << "\t<Function Definitions> ::= <Function> | <Function> <Function Definitions>" << endl;
	}

	F();
	FDpr();
}

void Parser::FDpr() {
	
	FD();

}

void Parser::F() {
	
	if (printRules)
	{
		cout << "\t<Function> ::= function <Identifier> [ <Opt Parameter List> ] <Opt Declaration List> <Body>" << endl;
		output << "\t<Function> ::= function <Identifier> [ <Opt Parameter List> ] <Opt Declaration List> <Body>" << endl;
	}

	//if (testToken.lexeme.compare("function") == 0)
	//{
		match("function");
		matchType("identifier");
		match("[");
		OPL();
		match("]");
		ODL();
		B();
	//}

}

void Parser::OPL() {
	
	if (printRules)
	{
		cout << "\t<Opt Parameter List> ::= <Parameter List> | <Empty>" << endl;
		output << "\t<Opt Parameter List> ::= <Parameter List> | <Empty>" << endl;
	}

	if (testToken.token.compare("identifier") == 0)
		PL();
	else
		return;
	
}

void Parser::PL() {
	
	if (printRules)
	{
		cout << "\t<Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>" << endl;
		output << "\t<Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>" << endl;
	}

	P();
	PLpr();

}

void Parser::PLpr() {

	//if (testToken.lexeme.compare(",") == 0)
	//{
		match(",");
		PL();
	//}
	//else
		return;
}

void Parser::P() {
	
	if (printRules)
	{
		cout << "\t<Parameter> ::= <IDs> : <Qualifier>" << endl;
		output << "\t<Parameter> ::= <IDs> : <Qualifier>" << endl;
	}
	
	IDS();

	if (testToken.lexeme.compare(":") == 0)
	{
		match(":");
		Q();
	}

}

void Parser::Q() {
	
	if (printRules)
	{
		cout << "\t<Qualifier> ::= int | boolean | real " << endl;
		output << "\t<Qualifier> ::= int | boolean | real " << endl;
	}

	if(testToken.lexeme.compare("int") == 0)
		match("int");
	else
	if(testToken.lexeme.compare("boolean") == 0)
		match("boolean");
	else
	if(testToken.lexeme.compare("real") == 0)
		match("real");

}

void Parser::B() {

	if (printRules)
	{
		cout << "\t<Body>  ::=  {  < Statement List>  }" << endl;
		output << "\t<Body>  ::=  {  < Statement List>  }" << endl;
	}
	
	if (testToken.lexeme.compare("{") == 0)
	{
		match("{");
		SL();
		match("}");
	}
}

void Parser::ODL() {
	
	if (printRules)
	{
		cout << "\t<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;
		output << "\t<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;
	}

	if(testToken.lexeme.compare("int") == 0 || testToken.lexeme.compare("boolean") == 0 || testToken.lexeme.compare("boolean") == 0)
		DL();
	else
		return;
}

void Parser::DL(){
	
	if (printRules)
	{
		cout << "\t<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;
		output << "\t<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl;
	}

	D();

	if (testToken.lexeme.compare(";") == 0)
	{
		match(";");
		DLpr();
	}

}

void Parser::DLpr()
{
}

void Parser::D()
{
}

void Parser::IDS()
{
	
}

void Parser::IDSpr()
{
}

void Parser::SL()
{
}

void Parser::SLpr()
{
}

void Parser::S()
{
}

void Parser::CMP()
{
}

void Parser::A()
{
}

void Parser::I()
{
}

void Parser::Ipr()
{
}

void Parser::R()
{
}

void Parser::Rpr()
{
}

void Parser::PR()
{
}

void Parser::SC()
{
}

void Parser::W()
{
}

void Parser::CND()
{
}

void Parser::RLP()
{
}

void Parser::E()
{
}

void Parser::Epr()
{
}

void Parser::T()
{
}

void Parser::Tpr()
{
}

void Parser::FA()
{
}

void Parser::PMY()
{
}

void Parser::EMP()
{
}
