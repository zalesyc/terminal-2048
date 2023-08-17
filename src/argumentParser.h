#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <string>
#include <vector>

class argumentParser {
  public:
    argumentParser(int argc, char const* argv[]);
    std::string getOption(const std::string option);
    bool optionExists(const std::string option);

  private:
    std::vector<std::string> m_args;
};

#endif // ARGUMENTPARSER_H
