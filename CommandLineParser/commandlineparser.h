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

#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <vector>
#include <string>
#include <functional>

using Validator = std::function<bool(std::string)>;

class CommandLineParser
{
  struct Argument
  {
    Argument(const std::string &name, const std::string &description,
             const std::string &hint)
      : name{name},
        hint{hint},
        description{description},
        value{""}
    {}
    std::string name;
    std::string hint;
    std::string description;
    std::string value;
  };

  struct Option
  {
    Option(const std::string &name, const std::vector<std::string> &alias,
           const std::string &description, const std::string &hint)
      : name{name},
        alias{alias},
        hint{hint},
        description{description},
        value{""},
        enabled{false}
    { }

    std::string name;
    std::vector<std::string> alias;
    std::string hint;
    std::string description;
    std::string value;
    bool enabled;
  };

public:
  enum class ErrorCode{
    NoError,
    InvalidOption};

  CommandLineParser();

  void addPositionalArgument(const std::string &argName, const std::string &description,
                             const std::string &hint = "");

  void addOption(const std::string &command, const std::string &description,
                 std::vector<std::string> alias = {}, const std::string &hint = "");


  bool process(const int &argc, char **argv);

  void showHelp() const;

  std::vector<std::string> args() const;
  std::string applicationName() const;


  std::string getErrorMessage() const;
  ErrorCode getErrorCode() const;
  std::pair<bool, std::string> isSet(const std::string &optionName) const;

  std::vector<Argument> getPositionalArgument() const;

private:

  ErrorCode m_errorCode;
  std::string m_applicationPath;
  std::vector<std::string> m_args;
  std::vector<Argument> m_positionalArgument;
  std::vector<Option> m_options;

  bool setOption(const std::string &name, const std::string &value);
  void setArgument(size_t position, const std::string &value);
};

#endif // COMMANDLINEPARSER_H
