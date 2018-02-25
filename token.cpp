// Contains Token Class:
// Holds information for the literal, object type, and string lexeme.
#include "token.h"
#include <iostream>
#include <iomanip>
using namespace std;

Token::Token(string lex,tokenType tt, string tStr){
  lexeme = lex;
  TT=tt;
  tokenTString = tStr;
}

std::pair<std::string,std::string> Token::outputToken(){
  pair<std::string,std::string> tokenPair = std::make_pair(lexeme,tokenTString);
  return tokenPair;
}
