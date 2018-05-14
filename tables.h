#ifndef SYMBOL_TABLE_HH
#define SYMBOL_TABLE_HH

#include <map>
#include <stack>

struct Symbol {
  std::string identifier_;
  std::string type_;
  size_t address_;
  Symbol(std::string id, const std::string &type, size_t address) :
    identifier_(id), type_(type), address_(address) {}
  Symbol() {}
};

struct Instruction {
  size_t address_;
  std::string op_;
  int operand_;
  Instruction(size_t address, const std::string &op, int operand) :
    address_(address), op_(op), operand_(operand) {}
  Instruction() {}
};

class SymbolTable {
private:
  const size_t STARTING_ADDRESS = 2000;
  size_t address_;
  
  std::map<int, Symbol> table_;

  std::string current_type_;
  bool is_duplicate(std::string &id);
public:
  SymbolTable();
  void insert(std::string &id);
  int get_address(std::string &token);
  bool inSymTable(std::string &token);
  void print();
  void set_current_type(const std::string &type);
};

class InstructionTable {
private:
  const size_t STARTING_INSTRUCTION = 1;
  size_t instruction_;
  std::stack<int> instruction_stack_;
  std::stack<int> jump_stack_;
  std::map<int, Instruction> table_;
public:
  InstructionTable();
  void gen_instr(const std::string &op, int operand);
  void print();
  void back_patch();
  void push_jump();
  size_t get_inst_num();
};

#endif