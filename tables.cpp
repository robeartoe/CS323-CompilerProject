#include "tables.h" //Yeah. Including this would definitely stop these errors. Whoops. haha
//-------------------------------------------------------------------------
//Symbol Table: //----------------------------------------------------------
// NOTE: extern makes these variables global variables. (I didn't know that. I'm still pretty rusty with C++)
// https://www.fayewilliams.com/2015/01/06/the-extern-keyword/
// UPDATE: Turns out you declare it here. But use the extern keyword in the parser.cpp file. So it'll know where this variable is.
// NOTE: "Note that if you include a definition AND the extern keyword, the extern is ignored"

int Memory_address = 2000;
std::map<int,sym>tableMap;

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
  return -1;
}

//----------------------------------------------------------
//InstructionTable: //----------------------------------------------------------

int instr_address = 1;
std::map<int,instr>Instr_table;

void gen_instr(std::string op,std::string oprd){
  instr newRow;
  newRow.address = instr_address;
  newRow.op = op;
  newRow.oprnd = oprd;
  Instr_table[instr_address] = newRow;
  instr_address++;
}

//----------------------------------------------------------
//Stacks: //----------------------------------------------------------
