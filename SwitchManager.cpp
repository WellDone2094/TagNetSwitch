//
// Created by Andrea Benfatti on 11/04/16.
//

#include <sstream>
#include "SwitchManager.h"
#include "Switch.h"

const std::string SwitchManager::executeCmd(std::string s) {
    std::istringstream iss(s);
    std::string cmd;
    iss >> cmd;

    if(methodMap.end() == methodMap.find(cmd)){
        return "invalid command";
    }

    int n;
    std::string s1, s2;
    Switch *sw = methodMap[cmd]->s;
    switch (methodMap[cmd]->t) {
        case VOID:
            return (sw->*methodMap[cmd]->void_m)();
            break;
        case INT:
            iss >> s1;
            try {
                n = std::stoi(s1, nullptr);
                return (sw->*methodMap[cmd]->int_m)(n);
            }catch (const std::invalid_argument& e){
                return "invalid argument";
            }
            break;
        case INT_STRING: iss >> s1 >> s2;
            try {
                n = std::stoi(s1, nullptr);
                return (sw->*methodMap[cmd]->int_string_m)(n, s2);
            }catch (const std::invalid_argument& e){
                return "invalid argument";
            }
            break;
    }
}

void SwitchManager::add_method(const std::string s, SwitchMethod* m) {
    methodMap[s] = m;
}

