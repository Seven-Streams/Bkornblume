#pragma once
#include<vector>
#include<string>

std::vector<std::string> Slice(std::string str, char delim = ' ') {
    std::vector<std::string> tokens;
    int last = 0;
    if((int)str.find(';') != -1) {
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