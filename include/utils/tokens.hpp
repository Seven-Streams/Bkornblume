#pragma once
#include <string>
#include <vector>

std::vector<std::string> Slice(std::string str, char delim = ' ') {
    std::vector<std::string> tokens;
    int last = 0;
    if (str.find(';') != std::string::npos) {
        str = str.substr(0, str.find(';'));
    }
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == delim) {
            tokens.push_back(str.substr(last, i - last));
            last = i + 1;
        }
    }
    tokens.push_back(str.substr(last, str.size() - last));
    return tokens;
}
