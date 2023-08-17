#include <algorithm>
#include <string>
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
