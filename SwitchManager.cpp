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

    int n1, n2;
    std::string s1, s2, s3;
    Switch *sw = methodMap[cmd]->s;
    switch (methodMap[cmd]->t) {
        case VOID:
            return (sw->*methodMap[cmd]->void_m)();
            break;
        case INT:
            iss >> s1;
            try {
                n1 = std::stoi(s1, nullptr);
                return (sw->*methodMap[cmd]->int_m)(n1);
            }catch (const std::invalid_argument& e){
                return "invalid argument";
            }
            break;
        case INT_STRING: iss >> s1 >> s2;
            try {
                n1 = std::stoi(s1, nullptr);
                return (sw->*methodMap[cmd]->int_string_m)(n1, s2);
            }catch (const std::invalid_argument& e){
                return "invalid argument";
            }
            break;
        case INT_INT_STRING: iss >> s1 >> s2 >> s3;
            try {
                n1 = std::stoi(s1, nullptr);
                n2 = std::stoi(s2, nullptr);
                return (sw->*methodMap[cmd]->int_int_string_m)(n1,n2, s3);
            }catch (const std::invalid_argument& e){
                return "invalid argument";
            }
            break;

    }
}

void SwitchManager::add_method(const std::string s, SwitchMethod* m) {
    methodMap[s] = m;
}

