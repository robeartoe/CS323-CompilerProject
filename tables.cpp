// SymTable::symTable(){
// }
//
// void SymTable::insert(int addr, std::string id, std::string t,std::string memLoc){
// }
//
// sym SymTable::at(int address){
// }
//
// void SymTable::print(){
//
// }

void insertSym(std::string type, std::string id){
  sym newRow;
  newRow.identifier = id;
  newRow.type = type;
  newRow.memLocation = Memory_address;
  tableMap[Memory_address] = newRow;
  Memory_address++;
}

sym get_address(std::string token){
	auto it = tableMap.begin();
	// Iterate through the map:
	while(it != tableMap.end()){
    // Checks the sym struct - identifier var and check if it matches.
		if(it->second.identifier == token){
      // Returns key value(AKA: Memory_address)
      return it->first;
		}
		// Go to next entry in map
		it++;
	}
}
//
// //----------------------------------------------------------
// //InstructionTable: //----------------------------------------------------------
// TODO: Apparently there's a get_instr function? Has to be a typo, but we should ask just to make sure.
/*
lexer();
E();
get_instr (POPM, get_address (save) );
end
*/

void gen_instr(std::string op,std::string oprd){
  instr newRow;
  newRow.address = instr_address;
  newRow.op = op;
  newRow.oprd = oprd;
  Instr_table[instr_address] = newRow;
  instr_address++;
}



// instrTable::instrTable(){
//
// }
//
// void instrTable::insert(int addr,std::string address,std::string op, std::string opr){
//
// }
// instr instrTable::at(int addr){
//
// }
// void instrTable::print(){
//
// }
