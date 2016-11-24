#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <vector>
#include <string>
#include <functional>

using Validator = std::function<bool(std::string)>;

class CommandLineParser
{

public:
  enum class ErrorCode{
    NoError,
    InvalidOption};

  CommandLineParser();

  void addPositionalArgument(const std::string &argName, const std::string &description,
                             const std::string &hint = "");

  void addOption(const std::string &command, const std::string &description,
                 std::vector<std::string> alias = {}, const std::string &hint = "");


  bool process(int &argc, char **argv);

  void showHelp();

  std::vector<std::string> args() const;
  std::string applicationName() const;


  std::string getErrorMessage() const;

  ErrorCode getErrorCode() const;

private:

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


  ErrorCode m_errorCode;
  std::string m_applicationPath;
  std::vector<std::string> m_args;
  std::vector<Argument> m_positionalArgument;
  std::vector<Option> m_options;

  void setOption(const std::string &name, const std::string &value);
  void setArgument(size_t position, const std::string &value);
};

#endif // COMMANDLINEPARSER_H
