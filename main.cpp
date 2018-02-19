//Main File to read file:

/*
The first assignment is to write a lexical analyzer (lexer)
You can build your entire lexer using a FSM, Or build using at least FSMs for identifier, integer and real (the rest can be written ad-hoc)
but YOU HAVE TO CONSTRUCT A FSM for this assignment otherwise, there will be a deduction of 2 points!
*/

#include <iostream>
#include <fstream>
#include <string>

#include "scanner.h"
using namespace std;

int main(){
  Scanner scan;
  string fileInputName,fileOutputName;

  std::cout << "Input Name of File: ";
  getline(cin,fileInputName);

  std::cout << "Input Name of Output File: ";
  getline(cin,fileOutputName);

  ifstream inputFile(fileInputName.c_str());
  ofstream outputFile(fileOutputName.c_str());

  if (inputFile.is_open()) {
    while (!inputFile.eof()) {
      // TODO: Complete Lexer/Scanner:
      // NOTE: So, it will pass the current string in the file, to the scanner. Then from there, the scanner will create an token object (or objects), and will push it onto the tokens vector. Let me know if you guys think of a more elegant solution.
      scan.lexer("a"); //"a" is just for testing.
    }

    // TODO: Output Results on Screen and on File:
    // Either do this outside of the while loop, or do it inside the while loop. Still not sure.

  }
  else{std::cout << "No Input File Found" << '\n';}
  inputFile.close();

  return 0;
}
