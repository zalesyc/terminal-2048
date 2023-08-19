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

std::pair<bool, std::string> argumentParser::getOption(const std::string option) {
    std::vector<std::string>::iterator itr;
    itr = std::find(m_args.begin(), m_args.end(), option);

    // return (itr != m_args.end() && ++itr != m_args.end()) ? *itr : "";
    if (itr == m_args.end()) {
        return std::make_pair(false, "");
    }

    if (++itr == m_args.end()) {
        return std::make_pair(true, "");
    }

    return std::make_pair(true, *itr);
}

bool argumentParser::optionExists(const std::string option) {
    return std::find(m_args.begin(), m_args.end(), option) != m_args.end();
}

bool argumentParser::setIntToOption(const std::string option, int* variableToBeSet, const std::pair<int, int> range) {
    std::pair<bool, std::string> argument = this->getOption(option);

    if (!argument.first) {
        return true;
    }

    if (argument.second.empty()) {
        std::cout << "Unexpected agument: \nMissing number parameter after the " << option << " option" << std::endl;
        return false;
    }

    if (!this->isNumber(argument.second)) {
        std::cout << "Unexpected agument: \nArgument after the " << option << " option must be a positive number between: " << range.first << " and " << range.second << std::endl;
        return false;
    }

    int intArgument = std::stoi(argument.second);
    
    if (intArgument < range.first || intArgument > range.second) {
        std::cout << "Unexpected agument: \nArgument after the " << option << " option must be a positive number between: " << range.first << " and " << range.second << std::endl;
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