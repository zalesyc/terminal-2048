#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "argumentParser.h"

argumentParser::argumentParser(int argc, char const* argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            m_args.push_back(argv[i]);
        }
    }
    // else error - no args provided
}

std::string argumentParser::getOption(const std::string option) {
    std::vector<std::string>::iterator itr;
    itr = std::find(m_args.begin(), m_args.end(), option);

    return (itr != m_args.end() && ++itr != m_args.end()) ? *itr : "";
}

bool argumentParser::optionExists(const std::string option) {
    return std::find(m_args.begin(), m_args.end(), option) != m_args.end();
}

bool argumentParser::setIntToOption(const std::string option, int* variableToBeSet, const std::pair<int, int> range) {
    std::string argument = this->getOption(option);
    if (argument.empty()) {
        // print error
        return false;
    }

    if (!this->isNumber(argument)) {
        // print error
        return false;
    }

    int intArgument = std::stoi(argument);
    std::cout << range.first << ", " << range.second << ", " << intArgument << std::endl;
    if (intArgument < range.first || intArgument > range.second) {
        // print error
        return false;
    }

    *variableToBeSet = intArgument;
    return true;
}

bool argumentParser::isNumber(const std::string string) {
    for (const char currentChar : string) {
        if (!std::isdigit(currentChar)) {
            return false;
        }
    }
    return true;
}