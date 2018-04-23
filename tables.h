// NOTE: I thought about making classes for the tables, then just thought to make it much simpler. So I'm keeping both, to see what you guys would like more.

#ifndef TABLES_H
#define TABLES_H

#include <string>
#include <map>
#include <iostream>

// NOTE: extern makes these variables global variables. (I didn't know that. I'm still pretty rusty with C++)
extern int Memory_address = 2000;
struct sym{
  std::string identifier;
  std::string type;
  int memLocation;
}

extern std::map<int,sym> tableMap;

sym get_address(std::string token);

// class symTable {
// private:
//   sym symbolRow;
//   std::map<int,sym> tableMap;
//
// public:
//   SymTable ();
//   void insert(int addr, std::string id, std::string t,std::string memLoc);
//
//   // Returns a symbol struct object.
//   sym at(int address);
//
//   // COUT all information in map.
//   // The key values will be the addresses. So by default it will be printed in order. 2000 and on.
//   void print();
// };

extern int instr_address = 1;

struct instr{
  std::string address;
  std::string op;
  std::string oprnd;
}

extern std::map<int,instr> Instr_table;

void gen_instr(std::string op,std::string oprd);

// TODO: Not sure where to implement the stacks.
void back_patch(int instr_address);

// class instrTable {
// private:
//   instr instructRow;
//   std::map<int,instr> instrMap;
// public:
//   instrTable ();
//   void insert(int addr,std::string address,std::string op, std::string opr);
//   instr at(int addr);
//   void print();
// };

#endif
/*
Every identifier declared  in the program should be placed in a symbol table and accessed by the symbol table handling procedures.
a) Each entry in the symbol table should hold the lexeme, and a "memory address" where an  identifier is placed within the symbol table.  For example, define a global integer variable called "Memory_address" and set initially 2000 and increment it by one when a new identifier is declared and placed into the table.
b) You need to write a procedure that will check to see if a particular identifier is already in the table, a procedure that will insert into the table and a procedure that will printout all identifiers in the table. If an identifier is used without declaring it, then the parser should provide an error message. Also, if an identifier is already in the table and wants to declare it for the second time, then the parser should provide an error message. Also, you should check the type match.

*/
