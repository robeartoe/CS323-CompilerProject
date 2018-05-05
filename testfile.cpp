#include "parser.h"
#include "tables.h"
#include <iomanip>
#include  <iostream>
#include <cstdlib>

//To compile: make -f makefile.dbg

int main(int argc, char const *argv[]) {
  /* code */
  std::cout << "This is a test: " << '\n';
  std::cout << "Memory_address is: "<< Memory_address << '\n';
  std::cout << "instr_address is: " << instr_address << '\n';
  std::cout << "OK, lets increment them: " << '\n';
  Memory_address++;
  instr_address++;
  std::cout << "Memory_address is: "<< Memory_address << '\n';
  std::cout << "instr_address is: " << instr_address << '\n';
  return 0;
}
