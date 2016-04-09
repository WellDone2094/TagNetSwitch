//
// Created by Andrea Benfatti on 08/04/16.
//

#ifndef TAGSWITCH_SWITCHMANAGER_H
#define TAGSWITCH_SWITCHMANAGER_H

#include <string>
#include <map>
#include <thread>

enum func_type{NO_ARGUMENT, INT_STRING, INT_T};
typedef void (*pVoidFunc)();
typedef void (*pIntFunc)(int);
typedef void (*pIntStringFunc)(int, std::string);


class SwitchManager {
public:
    void add_function(std::string s, pVoidFunc f);
    void add_function(std::string s, pIntFunc f);
    void add_function(std::string s, pIntStringFunc f);

    std::string executeCmd(std::string);


private:
    std::map<std::string, func_type> funcTypeMap;
    std::map<std::string, pVoidFunc> voidMap;
    std::map<std::string, pIntFunc> intMap;
    std::map<std::string, pIntStringFunc> intStringMap;

};


#endif //TAGSWITCH_SWITCHMANAGER_H
