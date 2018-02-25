//Main File to read file:

/*
The first assignment is to write a lexical analyzer (lexer)
You can build your entire lexer using a FSM, Or build using at least FSMs for identifier, integer and real (the rest can be written ad-hoc)
but YOU HAVE TO CONSTRUCT A FSM for this assignment otherwise, there will be a deduction of 2 points!
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <streambuf>

#include "scanner.h"
using namespace std;

int main(){
  string fileInputName,fileOutputName;

  std::cout << "Input Name of File: ";
  getline(cin,fileInputName);

  std::cout << "Input Name of Output File: ";
  getline(cin,fileOutputName);

  ifstream inputFile(fileInputName.c_str());
  ofstream outputFile(fileOutputName.c_str());

  if (inputFile.is_open() && outputFile.is_open()) {
    std::string fileString( (std::istreambuf_iterator<char>(inputFile)),(std::istreambuf_iterator<char>()));

    Scanner scan(fileString);

    scan.lexer();

    // Write to File and Display Tokens:
    // Why not do both at the same time?
    std::cout <<setw(0) << "Token" << setw(20) <<"Lexeme" << '\n';
    outputFile << setw(0)<< "Token" << setw(20) << "Lexeme" << '\n';
    std::cout << std::setfill('-') << setw(27) << '\n';
    outputFile << std::setfill('-') << setw(27) << '\n';
    std::pair<std::string,std::string> tokenPair;
    // Logic here is, get the vectors size/length. For each vector, have that token object, return a pair of strings, which is the lexeme and tokenType, to cout for the file and command line.
    for (int i = 0; i < scan.getTokensLength(); i++) {
      tokenPair = scan.outputToken(i);
      std::cout << setw(0)<< tokenPair.first << std::setw(20) <<tokenPair.second << '\n';
      outputFile << setw(0) << tokenPair.first << std::setw(20) <<tokenPair.second << '\n';
    }
    inputFile.close();
    outputFile.close();
  }
  else{std::cout << "No Input File Found" << '\n';}

  return 0;
}
