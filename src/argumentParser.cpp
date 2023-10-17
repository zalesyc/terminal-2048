#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "argumentParser.hpp"

ArgumentParser::ArgumentParser(int argc, char const* argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            m_args.emplace_back(argv[i]);
        }
    }
}

std::pair<bool, std::string> ArgumentParser::getOption(const std::string& option) {
    std::vector<std::string>::iterator itr;
    itr = std::find(m_args.begin(), m_args.end(), option);

    if (itr == m_args.end()) {
        return std::make_pair(false, "");
    }

    if (++itr == m_args.end()) {
        return std::make_pair(true, "");
    }

    return std::make_pair(true, *itr);
}

bool ArgumentParser::optionExists(const std::string& option) {
    return std::find(m_args.begin(), m_args.end(), option) != m_args.end();
}

bool ArgumentParser::setIntToOption(const std::string& option, int* variableToBeSet, const std::pair<int, int>& range) {
    std::pair<bool, std::string> argument = this->getOption(option);

    if (!argument.first) {
        return true;
    }

    if (argument.second.empty()) {
        std::cout << "Unexpected agument: \nMissing number parameter after the " << option << " option" << '\n';
        return false;
    }

    if (!ArgumentParser::isNumber(argument.second)) {
        std::cout << "Unexpected agument: \nArgument after the " << option << " option must be a positive number between: " << range.first << " and " << range.second << '\n';
        return false;
    }

    int intArgument = std::stoi(argument.second);

    if (intArgument < range.first || intArgument > range.second) {
        std::cout << "Unexpected agument: \nArgument after the " << option << " option must be a positive number between: " << range.first << " and " << range.second << '\n';
        return false;
    }

    *variableToBeSet = intArgument;
    return true;
}

bool ArgumentParser::isNumber(const std::string& string) {
    return std::all_of(string.begin(), string.end(), [](char number) { return std::isdigit(number); });
}