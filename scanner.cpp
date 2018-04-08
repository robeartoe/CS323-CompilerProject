/*
CPSC 323-02
Names:	Stephen Shinn
Michael Perna
Robert Ruiz

Project: Assignment 1
*/

#include "scanner.h"
using namespace std;

Scanner::Scanner(string fileName){
	
	input.open(fileName);
}

Token Scanner::lexer(){			//lexer fuction

	string lexeme;	//Strings that will contain data after processing
	string token;
	bool tokenFound = false;

	char testChar;
	testChar = input.get();

	while (!tokenFound)
	{
		if (testChar == EOF)
		{
			lexeme.push_back(testChar);
			return Token{token, lexeme};
		}
		else
		if (testChar == '!')
		{
			do {
				testChar = input.get();		//proceed to next character inside comment
			} while(testChar != '!');		//iterate through all of comment

			testChar = input.get();			//sets next character to be read
		}
		else
		if(isspace(testChar))				//proceed through all whitespace
		{
			do {
				testChar = input.get();
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

bool Scanner::isOpSepChar(char a)
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

bool Scanner::isKeyword(string lex)
{
	for (auto x : keywords)
		if (lex.compare(x) == 0)
			return true;
	return false;
}

bool Scanner::isOperator(string lex)
{
	for (auto x : operators)
		if(lex.compare(x) == 0)
			return true;
	return false;
}

bool Scanner::isSeparator(string lex)
{
	for(auto x : separators)
		if(lex.compare(x) == 0)
			return true;
	return false;
}

bool Scanner::iseof()
{
	if (input.peek() == EOF)
		return true;
	else
		return false;
}