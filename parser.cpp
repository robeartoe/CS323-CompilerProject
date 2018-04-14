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
			if(testChar == '\n')
					lineNum++;
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

void Parser::printProduction(std::string rule)
{
	cout << "\t" << rule << endl;
	output << "\t" << rule << endl;
}

bool Parser::cmpLex(std::string inp)
{
	if(testToken.lexeme.compare(inp) == 0)
		return true;
	else
		return false;
}

bool Parser::cmpTok(std::string inp)
{
	if(testToken.token.compare(inp) == 0)
		return true;
	else
		return false;
}

void Parser::matchLex(std::string lx)
{
	if (cmpLex(lx))
	{
		testToken = lexer();
		printToken(testToken);
	}
	else
	{
		errorLex(lx);
	}
}

void Parser::matchTok(std::string tok)
{
	if (cmpTok(tok))
	{
		testToken = lexer();
		printToken(testToken);
	}
	else
	{
		errorTok(tok);
	}
}

void Parser::errorLex(std::string tok)
{
	cout << "Error: Expected '" << tok << "' in line " << lineNum;
	output << "Error: Expected '" << tok << "' in line " << lineNum;
	exit(0);
}

void Parser::errorTok(std::string tok)
{
	cout << "Error: Expected " << tok << " in line " << lineNum;
	output << "Error: Expected " << tok << " in line " << lineNum;
	exit(0);
}


void Parser::R18S() 
{
	testToken = lexer();
	printToken(testToken);

	if(printRules)
		printProduction("<Rat18S> ::= <Opt Function Definitions> %% <Opt Declaration List> <Statement List>");

	OFD();
	if (cmpLex("%%"))
	{
		matchLex("%%");
		ODL();
		SL();
	}
	else
		errorLex("%%");

}

void Parser::OFD() 
{
	if(printRules)
		printProduction("<Opt Function Definitions> ::= <Function Definitions> | <Empty>");

	if(cmpLex("function"))
		FD();
}

void Parser::FD() 
{
	if(printRules)
		printProduction("<Function Definitions> ::= <Function> <Function Definitions>'");

	F();
	FDpr();
}

void Parser::FDpr() 
{
	if(printRules)
		printProduction("<Function Declaration>' ::= <Function Declaration> | <Empty>");

	if(cmpLex("function"))
		FD();
}

void Parser::F() 
{
	if(printRules)
		printProduction("<Function> ::= function <Identifier> [ <Opt Parameter List> ] <Opt Declaration List>  <Body>");


	if (cmpLex("function"))
	{
		matchLex("function");
		matchTok("identifier");
		matchLex("[");
		OPL();
		matchLex("]");
		ODL();
		B();
	}
	else
		errorLex("function");
	

}

void Parser::OPL() 
{
	if (printRules)
		printProduction("<Opt Parameter List> ::= <Parameter List> | <Empty>");

	if (cmpLex("identifier"))
	{
		PL();
	}
}

void Parser::PL() 
{
	if(printRules)
		printProduction("<Parameter List> ::= <Parameter> <Parameter List>'");

	P();
	PLpr();
}

void Parser::PLpr() 
{
	if(printRules)
		printProduction("<Parameter List>' ::= , <Parameter List> | <Empty>");

	if (cmpLex(","))
	{
		matchLex(",");
		PL();
	}
}

void Parser::P() 
{
	if(printRules)
		printProduction("<Parameter> ::= <IDs> : <Qualifier>");

	IDS();
	if (cmpLex(":"))
	{
		matchLex(":");
		Q();
	}
	else
		errorLex(":");
}

void Parser::Q() 
{
	if(printRules)
		printProduction("<Qualifier> ::= int | boolean | real");

	if (cmpLex("int") ||
		cmpLex("boolean") ||
		cmpLex("real"))
	{
		if(cmpLex("int"))
			matchLex("int");
		else
		if(cmpLex("boolean"))
			matchLex("boolean");
		else
			matchLex("real");
	}
	else
	{
		cout << "Error: Expected 'int', 'boolean', or 'real' in line " << lineNum;
		output << "Error: Expected 'int', 'boolean', or 'real' in line " << lineNum;
		exit(0);
	}

}

void Parser::B()
{
	if(printRules)
		printProduction("<Body> ::= { <Statement List> }");

	if (cmpLex("{"))
	{
		matchLex("{");
		SL();
		matchLex("}");
	}
	else
		errorLex("{");
	
}

void Parser::ODL()
{
	if(printRules)
		printProduction("<Opt Declaration List> ::= <Declaration List> | <Empty>");

	if (cmpLex("int") ||
		cmpLex("boolean") ||
		cmpLex("real"))
	{
		DL();
	}
}

void Parser::DL()
{
	if(printRules)
		printProduction("<Declaration List>  ::= <Declaration> ; <Declaration List>'");
	
	//if error come back here and fix
	D();
	if (cmpLex(";"))
	{
		matchLex(";");
		DL();
	}
	else
		errorLex(";");
	

}

void Parser::DLpr()
{
	if(printRules)
		printProduction("<Declaration List>' ::= <Declaration List> | <Empty>");

	if (cmpLex("int") ||
		cmpLex("boolean") ||
		cmpLex("real"))
	{
		DL();
	}
}

void Parser::D()
{
	if(printRules)
		printProduction("<Declaration> ::= <Qualifier> <IDs>");

	Q();
	IDS();
}

void Parser::IDS()
{
	if(printRules)
		printProduction("<IDs> ::= <Identifier> <IDs>'");

	if (cmpTok("identifier"))
	{
		matchTok("identifier");
		IDSpr();
	}
}

void Parser::IDSpr()
{
	if(printRules)
		printProduction("<IDs>' ::= , <IDs> | <Empty>");

	if (cmpLex(","))
	{
		matchLex(",");
		IDS();
	}
}

void Parser::SL()
{
	if(printRules)
		printProduction("<Statement List> ::= <Statement> <Statement List>'");

	S();
	SLpr();
}

void Parser::SLpr()
{
	if(printRules)
		printProduction("<Statement List>' ::= <Statement List> | <Empty>");

	if (cmpLex("{") ||
		cmpTok("identifier") ||
		cmpLex("if") ||
		cmpLex("return") ||
		cmpLex("put") ||
		cmpLex("get") ||
		cmpLex("while"))
	{
		SL();
	}
}

void Parser::S()
{
	if(printRules)
		printProduction("<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

	if(	cmpLex("{")				||
		cmpTok("identifier")	||
		cmpLex("if")			||
		cmpLex("return")		||
		cmpLex("put")			||
		cmpLex("get")			||
		cmpLex("while"))
	{ 
		if(cmpLex("{"))
			CMP();
		else
		if(cmpTok("identifier"))
			A();
		else
		if(cmpLex("if"))
			I();
		else
		if(cmpLex("return"))
			R();
		else
		if(cmpLex("put"))
			PR();
		else
		if(cmpLex("get"))
			SC();
		else
			W();
	}
	else
	{
		cout << "Error: Incorrect statement format in line " << lineNum;
		exit(0);
	}

}

void Parser::CMP()
{
	if(printRules)
		printProduction("<Compound> ::= { <Statement List> }");

	if (cmpLex("{"))
	{
		matchLex("{");
		SL();
		matchLex("}");
	}
	else
		errorLex("{");
}

void Parser::A()
{
	if(printRules)
		printProduction("<Assign> ::= <Identifier> = <Expression> ;");

	if (cmpTok("identifier"))
	{
		matchTok("identifier");
		matchLex("=");
		E();
		matchLex(";");
	}
	else
		errorTok("identifier");
}

void Parser::I()
{
	if(printRules)
		printProduction("<If> ::= if ( <Condition> ) <Statement> <If>'");

	if (cmpLex("if"))
	{
		matchLex("if");
		matchLex("(");
		CND();
		matchLex(")");
		S();
		Ipr();
	}
	else
		errorLex("if");
}

void Parser::Ipr()
{
	if(printRules)
		printProduction("<If>' ::= endif | else <Statement> endif");

	if(cmpLex("endif"))
		matchLex("endif");
	else
	if (cmpLex("else"))
	{
		matchLex("else");
		S();
		matchLex("endif");
	}
	else
	{
		cout << "Error: Expected 'else' or 'endif' in line " << lineNum;
		output << "Error: Expected 'else' or 'endif' in line " << lineNum;
		exit(0);
	}
}

void Parser::R()
{
	if(printRules)
		printProduction("<Return> ::= return <Return>'");

	if (cmpLex("return"))
	{
		matchLex("return");
		Rpr();
	}
	else
		errorLex("return");
}

void Parser::Rpr()
{
	if(printRules)
		printProduction("<Return>' ::= ; | <Expression> ;");

	if(cmpLex(";"))
		matchLex(";");
	else
	{
		E();
		matchLex(";");
	}
}

void Parser::PR()
{
	if(printRules)
		printProduction("<Print> ::= put ( <Expression> ) ;");

	if (cmpLex("put"))
	{
		matchLex("put");
		matchLex("(");
		E();
		matchLex(")");
		matchLex(";");
	}
	else
		errorLex("put");
}

void Parser::SC()
{
	if(printRules)
		printProduction("<Scan> ::= get ( <IDs> ) ;");

	if (cmpLex("get")) 
	{
		matchLex("get");
		matchLex("(");
		IDS();
		matchLex(")");
		matchLex(";");
	}
	else
		errorLex("get");
}

void Parser::W()
{
	if(printRules)
		printProduction("<While> ::= while ( <Condition> ) <Statement>");

	if (cmpLex("while"))
	{
		matchLex("while");
		matchLex("(");
		CND();
		matchLex(")");
		S();
	}
	else
		errorLex("while");
}

void Parser::CND()
{
	if(printRules)
		printProduction("<Condition> ::= <Expression> <Relop> <Expression>");

	E();
	RLP();
	E();
}

void Parser::RLP()
{
	if(printRules)
		printProduction("<Relop> ::= == | ^= | > | < | => | =<");

	if (cmpLex("==") ||
		cmpLex("^=") ||
		cmpLex(">")	 ||
		cmpLex("<")  ||
		cmpLex("=>") ||
		cmpLex("=<") )
	{
		if(cmpLex("=="))
			matchLex("==");
		else
		if(cmpLex("^="))
			matchLex("^=");
		else
		if(cmpLex(">"))
			matchLex(">");
		else
		if(cmpLex("<"))
			matchLex("<");
		else
		if(cmpLex("=>"))
			matchLex("=>");
		else
			matchLex("=<");
	}
	else
	{
		cout << "Error: Expected relational operator in line " << lineNum;
		output << "Error: Expected relational operator in line " << lineNum;
		exit(0);
	}
}

void Parser::E()
{
	if(printRules)
		printProduction("<Expression> ::= <Term> <Expression>'");

	T();
	Epr();
}

void Parser::Epr()
{
	if(printRules)
		printProduction("<Expression>' ::= + <Term> <Expression>' ");

	if (cmpLex("+"))
	{
		matchLex("*");
		Epr();
	}
	else
	if (cmpLex("-"))
	{
		matchLex("/");
		Epr();
	}

	return;
}

void Parser::T()
{
	if(printRules)
		printProduction("<Term> ::= <Factor> <Term>'");

	FA();
	Tpr();
}

void Parser::Tpr()
{
	if(printRules)
		printProduction("<Term>' ::= * <Factor> <Term>' | / <Factor> <Term>' | <Empty>");

	if (cmpLex("*"))
	{
		matchLex("*");
		Tpr();
	}
	else
	if (cmpLex("/"))
	{
		matchLex("/");
		Tpr();
	}

	return;
}

void Parser::FA()
{
	if(printRules)
		printProduction("<Factor> ::= - <Primary> | <Primary>");

	if (cmpLex("-"))
	{
		matchLex("-");
		PMY();
	}
	else
		PMY();
}

void Parser::PMY()
{
	if(printRules)
		printProduction("<Primary> ::= int | <Identifier> <Primary>' | ( <Expression> ) | real | true | false");

	
	if( cmpTok("int")			||
		cmpTok("real")			|| 
		cmpTok("true")			||
		cmpTok("false")			||
		cmpTok("identifier")	||
		cmpLex("(") )
	{ 
	
		if (cmpTok("int"))
		{
			matchTok("int");
		}
		else
		if (cmpTok("real"))
		{
			matchTok("real");
		}
		else
		if (cmpTok("true"))
		{
			matchTok("true");
		}
		else
		if (cmpTok("false"))
		{
			matchTok("false");
		}
		else
		if (cmpTok("identifier"))
		{
			matchTok("identifier");
			PMYpr();
		}
		else
		//if (testToken.lexeme.compare("(") == 0)
		{
			matchLex("(");
			E();
			if (cmpLex("("))
			{
				matchLex(")");
			}
			else
				errorLex(")");

			return;
		}
	}
	else
	{
		cout << "Error: Expected primary in line " << lineNum;
		output << "Error: Expected primary in line " << lineNum;
		exit(0);
	}

}

void Parser::PMYpr()
{
	if(printRules)
		printProduction("<Primary>' ::= ( <IDs> ) | <Empty> ");

	if (cmpLex("("))
	{
		matchLex("(");
		IDS();
		if (cmpLex(")"))
			matchLex(")");
		else
			errorLex(")");

	}

	return;
}

void Parser::EMP()
{
	if(printRules)
		printProduction("<Empty> ::= Epsilon");

	return;
}