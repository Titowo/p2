#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <algorithm>
#include <cctype>

namespace Utils {
    std::string toLower(const std::string& str);
    std::string cleanWord(std::string word);
};


#endif