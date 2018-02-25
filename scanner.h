#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include "token.h"

class Scanner {
private:
  std::string source;
  std::vector<Token> tokens;

public:
  Scanner(std::string fileString);
  void lexer();
  std::pair<std::string,std::string> outputToken(int index);
  int getTokensLength();
  void getTest();
};

#endif
