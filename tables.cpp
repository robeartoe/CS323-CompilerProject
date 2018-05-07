#include "tables.h"
//-------------------------------------------------------------------------
//Symbol Table: //----------------------------------------------------------
// NOTE: extern makes these variables global variables.
// NOTE: "Note that if you include a definition AND the extern keyword, the extern is ignored"

//TODO: If an identifier is used without declaring it, then the parser should provide an error message. Also, if an identifier is already in the table and wants to declare it for the second time, then the parser should provide an error message. Also, you should check the type match.

int Memory_address = 2000;
std::map<int,sym>tableMap;

// A procedure that will insert into the table
void insertSym(std::string type, std::string id){
  sym newRow;
  newRow.identifier = id;
  newRow.type = type;
  newRow.memLocation = Memory_address;
  tableMap[Memory_address] = newRow;
  Memory_address++;
}

int get_address(std::string token){
  for (auto &i : tableMap){
    if (i.second.identifier == token) {
      return i.first;
    }
  }
  return -1; //Will return -1 if address is NOT found.
}

//  A procedure that will check to see if a particular identifier is already in the table
bool inSymTable(std::string token){
  for (auto &i : tableMap){
    if (i.second.identifier == token) {
      return true;
    }
  }
  return false;
}

// A procedure that will printout all identifiers in the table.
void printSymTableIdentifiers(){
  for (auto &i : tableMap){
    std::cout << "Identifer: "<< i.second.identifier;
    std::cout << " Type: "<< i.second.type << std::endl;
  }
}

//----------------------------------------------------------
//InstructionTable: //----------------------------------------------------------


int instr_address = 1;
std::map<int,instr>Instr_table;

void gen_instr(std::string op,int oprd){
  instr newRow;
  newRow.address = instr_address;
  newRow.op = op;
  newRow.oprnd = oprd;
  Instr_table[instr_address] = newRow;
  instr_address++;
}

void printInstrTable(){
  for (auto &i : Instr_table){
    std::cout << "Address: "<< i.second.address;
    std::cout << " Operation: "<< i.second.op;
    std::cout << " Operand: "<< i.second.oprnd << std::endl;
  }
}

//----------------------------------------------------------
//Stacks: //----------------------------------------------------------
std::stack<int> instrStack;
std::stack<int> jumpStack;

void back_patch(int instr_address){
  int addr = jumpStack.top();
  jumpStack.pop();
  Instr_table[addr].oprnd = instr_address;
}
