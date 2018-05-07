/*
CPSC 323-02
Names:	Stephen Shinn
Michael Perna
Robert Ruiz

Project: Assignment 3
*/

#include "parser.h"
#include "tables.h"
#include <iomanip>
#include <string>
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
	if (cmpTok("EOF"))
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
	if(cmpTok("EOF"))
		printRules = false;
	else
		printRules = true;

	if (printRules && !cmpTok("EOF"))
	{
		cout << "Token: " << setw(15) << left  << x.token << "Lexeme: " << setw(15) << left << x.lexeme << endl;
		output << "Token: " << setw(15) << left  << x.token << "Lexeme: " << setw(15) << left << x.lexeme << endl;
	}
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

	EMP();
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

	EMP();
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

	if (cmpTok("identifier"))
		PL();

	EMP();
}

void Parser::PL()
{
	if(printRules)
		printProduction("<Parameter List> ::= <Parameter> <Parameter List>'");

	P();
	PLpr();
	EMP();
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
		DL();

	EMP();
}

void Parser::DL()
{
	if(printRules)
		printProduction("<Declaration List>  ::= <Declaration> ; <Declaration List>'");

	D();
	if (cmpLex(";"))
	{
		matchLex(";");
		DLpr();
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
		DL();

	EMP();
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
	else
		errorTok("identifier");
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

	EMP();
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
		SL();

	EMP();
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
	//TODO: Assignment 3 instructions.
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
		std::string save = "token";
		matchTok("identifier");
		matchLex("=");
		E();
		gen_instr("POPM",get_address(save));
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
		int addr = instr_address;
		matchLex("(");
		CND();
		matchLex(")");
		S();
		back_patch(instr_address);
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
		//I believe .... there's a back_patch around here?
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
  // TODO: Assignment 3 Instructions
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
	//TODO: Assignment 3 instructions
  //TODO: Can't find F() function?
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
		int addr = instr_address;
		gen_instr("LABEL",0);
		matchLex("(");
		CND();
		matchLex(")");
		S();
		gen_instr("JUMP",addr);
		//TODO: back_patch(instr_address);
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
		{
			gen_instr("EQU",0);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ",0);
			matchLex("==");
		}
		else
		if(cmpLex("^="))
		{
			gen_instr("NEQ",0);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ",0);
			matchLex("^=");
		}
		else
		if(cmpLex(">"))
		{
			gen_instr("GRT",0);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ",0);
			matchLex(">");
		}
		else
		if(cmpLex("<"))
		{
			gen_instr("LES",0);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ",0);
			matchLex("<");
		}
		else
		if(cmpLex("=>"))
		{
			gen_instr("GEQ",0);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ",0);
			matchLex("=>");
		}
		else{
			gen_instr("LEQ",0);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ",0);
			matchLex("=<");
		}
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
		matchLex("+");
		T();
		gen_instr("ADD",0); //Essentially 0 is NIL.
		Epr();
	}
	else
	if (cmpLex("-"))
	{
		matchLex("-");
		T();
		Epr();
	}

	EMP();
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
		FA();
		gen_instr("MUL",0);
		Tpr();
	}
	else
	if (cmpLex("/"))
	{
		matchLex("/");
		FA();
		Tpr();
	}

	EMP();
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


	if( cmpTok("integer")		||
		cmpTok("real")			||
		cmpLex("true")			||
		cmpLex("false")			||
		cmpTok("identifier")	||
		cmpLex("(") )
	{

		if (cmpTok("integer"))
		{
			matchTok("integer");
		}
		else
		if (cmpTok("real"))
		{
			matchLex("real");
		}
		else
		if (cmpLex("true"))
		{
			matchLex("true");
		}
		else
		if (cmpLex("false"))
		{
			matchLex("false");
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
			if (cmpLex(")"))
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
	EMP();
}

void Parser::EMP()
{
	if(printRules)
		printProduction("<Empty> ::= Epsilon");

	return;
}

void Parser::parseMsg()
{
	if (iseof())
	{
		cout << "Parsing completed successfully." << endl;
		output << "Parsing completed successfully." << endl;
	}
	else
	{
		cout << "End of file." << endl;
		output << "End of file." << endl;
	}
}
