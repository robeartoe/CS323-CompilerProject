// Contains Scanner Class
// This class parses through the text file and creates token objects.
#include "scanner.h"
#include <fstream>
using namespace std;

Scanner::Scanner(string fileName){
	
	input.open(fileName);
}

Token Scanner::lexer(){			//lexer fuction

	string token, lexeme;	//Strings that will contain data after processing
	bool tokenFound = false;
	char testChar, nextChar;	// Characters needed for processing tokens

	//Implement the tokenizer (create a token from the input stream to test with the FSMs)
	
	while (!tokenFound)
	{
		testChar = static_cast<char>(input.get());			//casts the int from input stream into a char and assigns it to testChar
		nextChar = static_cast<char>(input.peek());					// Stores the next character in the stream for decision making purposes below while not affecting stream file pointer position


		// This section processes the token if it is empty

		if (token.empty())
		{
			if (isOpSepChar(testChar))		// Checks to see if the first character to be added to token is a op/sep
			{
				token.push_back(testChar);	// Adds the character into the token string

				if (isSpecNextChar(testChar))	// This part determines if the character is part of a two-character relative operator or %% separator
				{
					tokenFound = false;			// Ensures the algorithm continues searching for token
				}
				else
					tokenFound = true;			// If test for two-char relop/%% op fails then it is a 1 character operator and is found
			}
			else
				if (isalpha(testChar))	// Checks to see if the first character to be added to token is an alphabet
				{
					token.push_back(testChar);	// Adds the alpha character into the token string

					if (nextChar == '$')		// Checks to see if the next character in the stream is '$'
						tokenFound = false;		// Ensures algorithm continues to search for token
					else
						if (isOpSepChar(nextChar) || isspace(nextChar))		// Checks next char to see if token has been found and assigns tokenFound accordingly
							tokenFound = true;
						else
							tokenFound = false;

				}
				else
					if (isdigit(testChar))		// Checks to see if first character to be added to token is a digit
					{
						token.push_back(testChar);	// Adds the digit character into the token string

						if (isalpha(nextChar))		// Checks to see if next character is an alphabet and ends the token string
							tokenFound = true;
						else
							if (isdigit(nextChar) || nextChar == '.')	// Checks to see if next character continues to valid trend of a real/integer
								tokenFound = false;
							else						// Unrecognized characters will end token processing
								tokenFound = true;

					}
					else
						if (!isspace(testChar))		// Checks to see if the current character is unrecognized, and adds to the empty token
							token.push_back(testChar);
						else						// Whitespace will be ignored and nothing will be added to the token string
							tokenFound = false;
		}
		else
		// This section will process the token when it contains something
		{

		}


	}

	//Use the two FSMs which our string value will be compared to (PENDING)

	if (isdigit(token.at(0)))
		return intRealFSM(token);
	else
		if (isalpha(token.at(0)))
			return idFSM(token);
		else
			return Token{ token, "unrecognized" };
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
		case '!':
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
		case '$':
			return true;
		default:
			break;
	}

	return false;

	return 0;
}

bool Scanner::isSpecNextChar(char a)
{
	char next = static_cast<char>(input.peek());
	if (a == '=')
		switch (next)
		{
		case '>':
		case '<':
		case '=':
			return true;
		}
	else
		if (a == '^')
		{
			if (next == '=')
				return true;
		}
		else
			if (a == '%')
			{
				if (next == '%')
					return true;
			}
			else
				return false;

}

bool Scanner::lookAhead(char a)
{

	return false;
}

bool Scanner::iseof()
{
	if (input.eof())
		return true;
	else
		return false;
}

Token Scanner::intRealFSM(string tkn)
{
	int state = 1;
	string lx;

	for(int i = 0; i < tkn.length(); i++)
		switch (state)
		{
			case 1:
				if (isdigit(tkn.at(i)))
				{
					state = 2;
					break;
				}
				else
				if (tkn.at(i) == '.')
				{
					state = 5;
					break;
				}
				break;
			case 2:
				if (isdigit(tkn.at(i)))
				{
					state = 2;
					break;
				}
				else
				if (tkn.at(i) == '.')
				{
					state = 3;
					break;
				}
				break;
			case 3:
				if (isdigit(tkn.at(i)))
				{
					state = 4;
					break;
				}
				else
				if (tkn.at(i) == '.')
				{
					state = 5;
					break;
				}
				break;
			case 4:
				if (isdigit(tkn.at(i)))
				{
					state = 4;
					break;
				}
				else
				if (tkn.at(i) == '.')
				{
					state = 5;
					break;
				}
				break;
			case 5:
				if (isdigit(tkn.at(i)))
				{
					state = 5;
					break;
				}
				else
				if (tkn.at(i) == '.')
				{
					state = 5;
					break;
				}
				break;
		}

	switch (state)
	{
		case 1:
		case 3:
		case 5:
		default:
			lx = "unrecognized";
			break;
		case 2:
			lx = "integer";
			break;
		case 4:
			lx = "real";
			break;
	}

	return Token{ tkn, lx };
}

Token Scanner::idFSM(string tkn)
{
	int state = 1;
	string lx;

	for(int i = 0; i < tkn.length(); i++)
		switch (state)
		{
			case 1:
				if (isalpha(tkn.at(i)))
				{
					state = 2;
					break;
				}
				else if (isdigit(tkn.at(i)))
				{
					state = 6;
					break;
				}
				else if (tkn.at(i) = '$')
				{
					state = 6;
					break;
				}
				break;
			case 2:
				if (isalpha(tkn.at(i)))
				{
					state = 4;
					break;
				}
				else if (isdigit(tkn.at(i)))
				{
					state = 5;
					break;
				}
				else if (tkn.at(i) = '$')
				{
					state = 3;
					break;
				}
				break;
			case 3:
				if (isalpha(tkn.at(i)))
				{
					state = 6;
					break;
				}
				else if (isdigit(tkn.at(i)))
				{
					state = 6;
					break;
				}
				else if (tkn.at(i) = '$')
				{
					state = 6;
					break;
				}
				break;
			case 4:
				if (isalpha(tkn.at(i)))
				{
					state = 4;
					break;
				}
				else if (isdigit(tkn.at(i)))
				{
					state = 5;
					break;
				}
				else if (tkn.at(i) = '$')
				{
					state = 3;
					break;
				}
				break;
			case 5:
				if (isalpha(tkn.at(i)))
				{
					state = 4;
					break;
				}
				else if (isdigit(tkn.at(i)))
				{
					state = 5;
					break;
				}
				else if (tkn.at(i) = '$')
				{
					state = 3;
					break;
				}
				break;
			case 6:
				if (isalpha(tkn.at(i)))
				{
					state = 6;
					break;
				}
				else if (isdigit(tkn.at(i)))
				{
					state = 6;
					break;
				}
				else if (tkn.at(i) = '$')
				{
					state = 6;
					break;
				}
				break;
		}

		switch (state)
		{
			case 1:
			case 5:
			case 6:
			default:
				lx = "unrecognized";
				break;
			case 2:
			case 3:
				lx = "identifier";
				break;
			case 4:
				lx = "identifier";
			//IMPLEMENT KEYWORD PART HERE

				break;
		}

	return Token{ tkn, lx };
}
