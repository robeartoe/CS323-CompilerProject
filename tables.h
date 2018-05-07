#ifndef TABLES_H
#define TABLES_H

#include <string>
#include <map>
#include <stack>
#include <iostream>

//Symbol Table:

extern int Memory_address;

struct sym{
  std::string identifier;
  std::string type;
  int memLocation;
};

extern std::map<int,sym>tableMap;

int get_address(std::string token);

bool inSymTable(std::string token);

void printSymTableIdentifiers();

//Instruction Table:

extern int instr_address;
struct instr{
  int address;
  std::string op;
  int oprnd;
};

extern std::map<int,instr>Instr_table;

void gen_instr(std::string op,int oprd);

void printInstrTable();

//Stacks:
void back_patch(int instr_address);

extern std::stack<int> instrStack;
extern std::stack<int> jumpStack;

#endif
/*
Every identifier declared  in the program should be placed in a symbol table and accessed by the symbol table handling procedures.
a) Each entry in the symbol table should hold the lexeme, and a "memory address" where an  identifier is placed within the symbol table.  For example, define a global integer variable called "Memory_address" and set initially 2000 and increment it by one when a new identifier is declared and placed into the table.
b) You need to write a procedure that will check to see if a particular identifier is already in the table, a procedure that will insert into the table and a procedure that will printout all identifiers in the table. If an identifier is used without declaring it, then the parser should provide an error message. Also, if an identifier is already in the table and wants to declare it for the second time, then the parser should provide an error message. Also, you should check the type match.

*/
