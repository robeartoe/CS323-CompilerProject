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
		nextChar = static_cast<char>(input.peek());			// Stores the next character in the stream for decision making purposes below while not affecting stream file pointer position


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
				{
					tokenFound = true;
				}
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
						{
							token.push_back(testChar);
							tokenFound = true;
						}
						else						// Whitespace will be ignored and nothing will be added to the token string
							tokenFound = false;
		}
		else
		// This section will process the token when it contains something
		{
			if (isOpSepChar(token.at(0)) && isOpSepChar(testChar))	// Checks to see if prevChar was a op/sep and testChar to see if it is also an op/sep (Condition for continuing token taken care of in first section)
			{
				token.push_back(testChar);
				tokenFound = true;
			}
			else
				if (isalpha(token.at(0)))	// Checking to see if non-empty token begins with alpha
				{
					if (isalpha(testChar) || isdigit(testChar))
					{
						token.push_back(testChar);
						tokenFound = false;
					}
					else
					if (testChar == '$')
					{
						token.push_back(testChar);
						tokenFound = true;
					}
					else
					{
						input.unget();
						tokenFound = true;
					}
				}
				else
					if (isdigit(token.at(0)))	// Checking to see if non-empty token begins with digit
					{
						if (isdigit(testChar))
						{
							token.push_back(testChar);
							tokenFound = false;
						}
						else
							if (testChar == '.')
							{
								bool dotExists = false;						// This section tests to see if tokens starting with digit already have a dot in them, then assigns tokenFound accordingly
								for (int i = 0; i < token.length(); i++)
									if (token.at(i) == '.')
										dotExists = true;

								if (dotExists)
								{
									input.unget();						// After seeing dot exists, put the . back into the stream to be processed again at token reset
									tokenFound = true;
								}
								else
								{
									token.push_back(testChar);
									tokenFound = false;
								}

							}
							else
							{
								input.unget();
								tokenFound = true;	// All other unrecognized characters will make sure token ends here
							}

					}
					else
						if (isUnrecognized(token.at(0)) && isUnrecognized(testChar))
						{
							token.push_back(testChar);
							tokenFound = false;
						}
						else
							tokenFound = true;

		}


	}

	//Use the two FSMs which our string value will be compared to (PENDING)

	if (isdigit(token.at(0)))
		return intRealFSM(token);
	else
		if (isalpha(token.at(0)))
			return idFSM(token);
		else
			if (isOpSepChar(token.at(0)))
				return opSepCheck(token);
			else
				return Token{ token, "unrecognized" };
}

bool Scanner::isUnrecognized(char a)
{
	if (isalpha(a) || isdigit(a) || isOpSepChar(a))
		return false;
	else
		return true;

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

bool Scanner::isKeyword(string tkn)
{
	for (int i = 0; i < keywords.size(); i++)
		if (tkn.compare(keywords[i]) == 0)
			return true;

	return false;
	
}

bool Scanner::lookAhead(char a)
{

	return false;
}

bool Scanner::iseof()
{
	char c = static_cast<char>(input.get());
	if (c == EOF || c == -1)
		return true;
	else
		return false;
}

Token Scanner::opSepCheck(string tkn)
{
	string lx;
	char check = tkn.at(0);

		if (tkn.size() == 1)
		{
			switch (check)
			{
			case '+':
			case '-':
			case '*':
			case '/':
			case '<':
			case '>':
			case '=':
				lx = "operator";
				break;
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
				lx = "separator";
				break;
			default:
				lx = "unrecognized";
				break;
			}
		}
		else
			if (tkn.size() == 2)
			{
				switch (check)
				{
				case '=':
					if (tkn.at(1) == '>' || tkn.at(1) == '<' || tkn.at(1) == '=')
						lx = "operator";
					break;
				case '^':
					if (tkn.at(1) == '=')
						lx = "operator";
					break;
				case '%':
					if (tkn.at(1) == '%')
						lx = "separator";
					break;
				default:
					lx = "unrecognized (opSepCheck str size 2)";
					break;
				}
			}
			else
				lx = "unrecognized";
	
	return Token{ tkn, lx };
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
				if (isKeyword(tkn))
					lx = "keyword";
				break;
		}

	return Token{ tkn, lx };
}
