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
  Scanner();
  void lexer(std::string value);
  void outputTokens();
  void getTest();
};

#endif
