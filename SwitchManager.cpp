//
// Created by Andrea Benfatti on 08/04/16.
//

#include <sstream>
#include <iostream>
#include "SwitchManager.h"

void SwitchManager::add_function(std::string s, pVoidFunc f) {
    funcTypeMap[s] = NO_ARGUMENT;
    voidMap[s] = f;
}

void SwitchManager::add_function(std::string s, pIntFunc f) {
    funcTypeMap[s] = INT_T;
    intMap[s] = f;
}

void SwitchManager::add_function(std::string s, pIntStringFunc f) {
    funcTypeMap[s] = INT_STRING;
    intStringMap[s] = f;
}

std::string SwitchManager::executeCmd(std::string s) {
    std::istringstream iss(s);
    std::string cmd;
    iss >> cmd;

    std::map<std::string, func_type>::iterator res = funcTypeMap.find(cmd);
    if(res == funcTypeMap.end()){
        return std::string("invalid command");
    }

    int n;
    std::string s1, s2;
    switch (funcTypeMap[cmd]) {
        case NO_ARGUMENT:
            voidMap[cmd]();
            break;
        case INT_STRING:
            iss >> s1 >> s2;
            n = std::stoi(s1, nullptr);
            intStringMap[cmd](n, s2);
            break;
        case INT_T:
            iss >> s1;
            n = std::stoi(s1, nullptr);
            intMap[cmd](n);
            break;
    }

    return s1;
}
