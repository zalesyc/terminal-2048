#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <string>
#include <utility>
#include <vector>

class ArgumentParser {
  public:
    ArgumentParser(int argc, char const* argv[]);
    std::pair<bool, std::string> getOption(const std::string& option);
    bool optionExists(const std::string& option);
    bool setIntToOption(const std::string& option, int* variableToBeSet, const std::pair<int, int>& range);

  private:
    std::vector<std::string> m_args;
    static bool isNumber(const std::string& string);
};

#endif // ARGUMENTPARSER_H
