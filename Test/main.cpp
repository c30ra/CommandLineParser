#include <iostream>

#include "commandlineparser.h"
#include <algorithm>
#include <regex>
#include <sstream>

int main(int argc, char *argv[])
{
  CommandLineParser parser;

  parser.addPositionalArgument("source", "Input file", "file-path");

  if(!parser.process(argc, argv)){
      std::cout << parser.getErrorMessage() << '\n';
    }

  return 0;
}
