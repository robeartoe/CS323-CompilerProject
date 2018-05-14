#include "tables.h"
#include <iostream>

SymbolTable::SymbolTable() : address_(STARTING_ADDRESS) {}

void SymbolTable::insert(std::string &id) {
  if (is_duplicate(id)) {
    std::cout << id + " previously declared" << std::endl;
    exit(1);
  }
  
  Symbol newRow(current_type_, id, address_++);
  table_[newRow.address_] = newRow;
}

void SymbolTable::set_current_type(const std::string &type) {
  current_type_ = type;
}

int SymbolTable::get_address(std::string &token) {
  for (auto &i : table_) {
    if (i.second.identifier_ == token) {
      return i.first;
    }
  }
  return -1;
}

bool SymbolTable::inSymTable(std::string &token) {
  for (auto &i : table_) {
    if (i.second.identifier_ == token) {
      return true;
    }
  }
  return false;
}

void SymbolTable::print() {
  for (auto &i : table_) {
    std::cout << "Identifer: "<< i.second.identifier_ << std::endl;
    std::cout << "Type: "<< i.second.type_ << std::endl;
  }
}

bool SymbolTable::is_duplicate(std::string &id) {
  for (auto el : table_) {
    if (el.second.identifier_.compare(id) == 0) {
      return true;
    }
  }

  return false;
}


InstructionTable::InstructionTable() : instruction_(STARTING_INSTRUCTION) {}

void InstructionTable::gen_instr(const std::string &op, int operand) {
  Instruction newRow(instruction_++, op, operand);
  table_[newRow.address_] = newRow;
}

void InstructionTable::push_jump() {
  jump_stack_.push(instruction_++);
}

void InstructionTable::print() {
  for (auto &i : table_) {
    std::cout << "Address: " << i.second.address_ << std::endl;
    std::cout << " Operation: "<< i.second.op_ << std::endl;
    std::cout << " Operand: "<< i.second.operand_ << std::endl;
  }
}

void InstructionTable::back_patch() {
  size_t addr = jump_stack_.top();
  jump_stack_.pop();
  table_[addr].operand_ = instruction_;
}

size_t InstructionTable::get_inst_num() {
  return instruction_;
}