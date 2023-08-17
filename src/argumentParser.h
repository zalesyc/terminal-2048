#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <string>
#include <utility>
#include <vector>

class argumentParser {
  public:
    argumentParser(int argc, char const* argv[]);
    std::string getOption(const std::string option);
    bool optionExists(const std::string option);
    bool setIntToOption(const std::string option, int* variableToBeSet, const std::pair<int, int> range);

  private:
    std::vector<std::string> m_args;
    bool isNumber(const std::string string);
};

#endif // ARGUMENTPARSER_H
