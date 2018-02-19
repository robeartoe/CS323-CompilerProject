// Contains Token Class:
// Holds information for the literal, object type, and string lexeme.
#include "token.h"
#include <iostream>
using namespace std;

Token::Token(string lex,tokenType tt)
{
  lexeme = lex;
  TT=tt;
}
