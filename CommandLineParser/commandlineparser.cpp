/*******************************************************************************
This file is part of the CommadLineParser project.
Copyright (C) 2016  Luca Carella <bkarelb at hotmail dot it>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

#include "commandlineparser.h"

#include <algorithm>
#include <regex>
#include <iostream>

CommandLineParser::CommandLineParser()
  : m_errorCode{ErrorCode::NoError},
    m_applicationPath{ "" }
{
  addOption("h", "Show help", {"help"});
}

void CommandLineParser::addPositionalArgument(const std::string &argName,
                                              const std::string &description,
                                              const std::string &hint)
{
  m_positionalArgument.emplace_back(argName, description, hint);
}

void CommandLineParser::addOption(const std::string& command, const std::string& description,
                                  std::vector<std::string> alias, const std::string &hint)
{

  auto appendChar = [](const std::string &str){
      auto _name = str;
      if (str.length() == 1) {
          _name.insert(0, 1, '-');
        } else {
          _name.insert(0, "--");
        }
      return _name;
    };

  auto name = appendChar(command);

  for(auto &str : alias){
      str = appendChar(str);
    }

  m_options.emplace_back(name, alias, description, hint);
}

bool CommandLineParser::process(const int& argc, char** argv)
{
  static const std::regex shortOptionWvalue("(^-{1}[a-zA-Z]{1})((=|\\s)([^\\0]+))?",
                                            std::regex::optimize);
  static const std::regex longOptionWvalue("(^-{2}[a-zA-Z0-9]+)((=|\\s)([^\\0]+))?",
                                           std::regex::optimize);

  m_applicationPath = argv[0];

  std::vector<std::string> positionalArgs;
  std::vector<std::string> options;

  for (auto i{1}; i < argc; ++i) {
      // is an option
      if(argv[i][0] == '-'){
          auto str = std::string(argv[i]);

          // next arg is the value of the option
          auto nextArg = i + 1;
          if((nextArg < argc) && (argv[nextArg][0] != '-')){
              ++i; //skip next arg, cause it's the option value
              str += ' ';
              str += argv[nextArg];
            }
          options.emplace_back(str);
        }

      else
        positionalArgs.emplace_back(argv[i]);
    }

  // if -h or --help is found process it first and return
  auto found = std::end(options) != std::find(std::begin(options), std::end(options), "-h");
  found |= std::end(options) != std::find(std::begin(options), std::end(options), "--help");

  if(found){
      showHelp();
      return true;
    }

  std::smatch subMatches;
  auto optionCheck = true;
  for (const auto& opt : options) {
      // is short option
      if (std::regex_match(opt, subMatches, shortOptionWvalue)) {
          if (subMatches.size() == 5) {
              auto name = subMatches[1].str();
              auto value = subMatches[4].str();
              if(!setOption(name, value))
                optionCheck &= false;
            }

        }
      // is long option
      else if (std::regex_match(opt, subMatches, longOptionWvalue)) {
          if (subMatches.size() == 5) {
              auto name = subMatches[1].str();
              auto value = subMatches[4].str();
              if(!setOption(name, value))
                optionCheck &= false;
            }
        }
    }


  for(size_t i{0}; i < positionalArgs.size(); ++i) {
      setArgument(i, positionalArgs.at(i));
    }

  return optionCheck;
}

std::vector<std::string> CommandLineParser::args() const
{
  return m_args;
}

std::string CommandLineParser::applicationName() const
{
  return m_applicationPath;
}

std::string CommandLineParser::getErrorMessage() const
{
  auto errorString = std::string{"Error: "};
  switch(m_errorCode){
    case ErrorCode::NoError:
      errorString += "no error";
      break;
    case ErrorCode::InvalidOption:
      errorString += "invalid option";
      break;
    }
  return errorString;
}

CommandLineParser::ErrorCode CommandLineParser::getErrorCode() const
{
  return m_errorCode;
}

std::pair<bool, std::string> CommandLineParser::isSet(const std::string &optionName) const
{
  auto appendChar = [](const std::string &str){
      auto _name = str;
      if (str.length() == 1) {
          _name.insert(0, 1, '-');
        } else {
          _name.insert(0, "--");
        }
      return _name;
    };

  auto name = appendChar(optionName);

  auto searchResult =
      std::find_if(std::begin(m_options),
                   std::end(m_options),
                   [&name](const Option &option)
              {
                if (name == option.name) {
                    return true;
                }
                auto searchResult = std::find(std::begin(option.alias),
                                                std::end(option.alias), name);

                if (searchResult != std::end(option.alias)) {
                    return true;
                }

                return false;});

  if(std::end(m_options) != searchResult)
    return std::make_pair(searchResult->enabled, searchResult->value);
  else
    return std::make_pair(false, "");
}

std::vector<CommandLineParser::Argument> CommandLineParser::getPositionalArgument() const
{
  return m_positionalArgument;
}

bool CommandLineParser::setOption(const std::string &name, const std::string &value)
{
  auto searchResult =
      std::find_if(std::begin(m_options),
                   std::end(m_options),
                   [&name](const Option &option)
              {
                if (name == option.name) {
                    return true;
                }
                auto searchResult = std::find(std::begin(option.alias),
                                                std::end(option.alias), name);

                if (searchResult != std::end(option.alias)) {
                    return true;
                }

                return false;});

  if(searchResult != std::end(m_options)) {
      if(!value.empty()) {
          searchResult->enabled = true;
          searchResult->value = value;
        }
      else {
          searchResult->enabled = true;
        }
      return true;
    }
  else {
      m_errorCode = ErrorCode::InvalidOption;
      return false;
    }
}

void CommandLineParser::setArgument(size_t position, const std::string &value)
{
  if(position < m_positionalArgument.size()){
      auto arg = m_positionalArgument.at(position);
      arg.value = value;
    }
}

void CommandLineParser::showHelp() const
{
  auto applicationName = m_applicationPath;
  auto pos = applicationName.find_last_of('\\');
  if(pos != std::string::npos){
      applicationName.erase(0, pos + 1);
    }
  auto identationLevel{2};
  std::string identation(static_cast<size_t>(identationLevel), ' ');

  std::cout << "Usage " << '\n' << '\n'
            << "  " << applicationName << ' ';

  for(const auto &arg : m_positionalArgument){
      std::cout << arg.name << ' ';
    }

  std::cout << "[options]" << '\n' << '\n' << '\n';

  static const auto absoluthDescrPos{25};

  auto writeDescr = [](int offset, int position, const std::string &description){
      auto descrPos = position - offset;
      if(descrPos < 0){
          std::cout << '\n';
          descrPos = position;
        }
      std::cout << std::string(static_cast<size_t>(descrPos), ' ')
                << description << '\n';
    };

  if(!m_options.empty()){
      std::cout << "Options:" << '\n';
      for(const auto &opt : m_options){
          std::string name = (identation + opt.name);
          //add alias to string
          for(const auto &alias : opt.alias){
              name += ' ' + alias;
            }
          if(!opt.hint.empty()){
              name += ' ';
              name += '<' + opt.hint + '>';
            }
          std::cout << name;
          writeDescr(static_cast<int>(name.length()) - identationLevel,
                     absoluthDescrPos, opt.description);
        }
      std::cout << '\n';
    }

  if(!m_positionalArgument.empty()){
      std::cout << "Arguments:" << '\n';
      for(const auto &arg : m_positionalArgument){
          std::string name = (identation + arg.name);
          if(!arg.hint.empty()){
              name += ' ';
              name += '<' + arg.hint + '>';
            }
          std::cout << name;
          writeDescr(static_cast<int>(name.length()) - identationLevel,
                     absoluthDescrPos, arg.description);
        }

      std::cout << '\n';
    }

}
