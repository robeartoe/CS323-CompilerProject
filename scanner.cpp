// Contains Scanner Class
// This class parses through the text file and creates token objects.
#include "scanner.h"
#include "token.h"
#include <iostream>
using namespace std;

Scanner::Scanner(string fileString) : source(fileString) {};

void Scanner::lexer(){
  // The Magic Takes Place Here:
  // Token newValue();

}

std::pair<std::string,std::string> Scanner::outputToken(int index){
  return tokens[index].outputToken();
}

const int Scanner::getTokensLength() const {
  return tokens.size();
}



void Scanner::getTest(){
  std::cout << "Pretty bad at C++ I forget how to do to this..." << '\n';
}
