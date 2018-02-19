#ifndef TOKEN_H
#define TOKEN_H

#include <string>


class Token {
private:
  /* data */

public:
  typedef enum {
  // Single Character Tokens:
  LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY, COMMA, DOT, MINUS, PLUS,SEMICOLON,SLASH,MULT,
  //One or Two Char Tokens:
  EXCLAM,EXCLAM_EQUAL,EQUAL,EQUAL_EQUAL,GREATER,GREATER_EQUAL,LESS,LESS_EQUAL, MODULO,
  //Literals
  IDENTIFIER,INTEGER,REAL,
  //Keywords:
  INT,IF,ELSE,ENDIF,WHILE,RETURN,GET,PUT

  }tokenType;

  tokenType TT;
  std::string lexeme;

  // TODO: Figure out how to get string into the correct object type:
  // static const auto literal;

  Token(std::string lex,tokenType tt);

};

#endif
