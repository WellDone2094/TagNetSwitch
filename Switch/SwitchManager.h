//
// Created by Andrea Benfatti on 11/04/16.
//

#ifndef TAGSWITCH_SWITCHMANAGER_H
#define TAGSWITCH_SWITCHMANAGER_H

#include <string>
#include <map>
#include <thread>
#include <vector>
#include "ServerConnection.h"

class Switch;

enum type {
    VOID, INT, STRING, INT_STRING, INT_INT_STRING, INT_INT_LIST
};

struct SwitchMethod {
    SwitchMethod(Switch* o, const std::string (Switch::* m)()) : s(o), t(VOID), void_m(m) {}
    SwitchMethod(Switch* o, const std::string (Switch::* m)(int)) : s(o), t(INT), int_m(m) {}
    SwitchMethod(Switch* o, const std::string (Switch::* m)(const std::string &)) : s(o), t(STRING), string_m(m) {}
    SwitchMethod(Switch* o, const std::string (Switch::* m)(int, const std::string &)) : s(o), t(INT_STRING), int_string_m(m) {}
    SwitchMethod(Switch* o, const std::string (Switch::* m)(int, int, const std::string &)) : s(o), t(INT_INT_STRING), int_int_string_m(m) {}
    SwitchMethod(Switch* o, const std::string (Switch::* m)(int, int, std::vector<std::string>&)) : s(o), t(INT_INT_LIST), int_int_list_m(m) {}

    enum type t;
    Switch *s;
    union {
        const std::string (Switch::* string_m)(const std::string &);
        const std::string (Switch::* int_m)(int);
        const std::string (Switch::* void_m)();
        const std::string (Switch::* int_string_m)(int, const std::string &);
        const std::string (Switch::* int_int_string_m)(int, int, const std::string &);
        const std::string (Switch::* int_int_list_m)(int, int, std::vector<std::string> &);
    };
};

class SwitchManager {
public:
    SwitchManager(uint16_t port) : sc(port){
        server = std::thread(&SwitchManager::TCPServer, this);
    };
    void add_method(const std::string s, SwitchMethod* m);
    void quit();
    void TCPServer();

    const std::string executeCmd(std::string);

    std::thread server;
private:
    std::map<std::string, SwitchMethod*> methodMap;
    bool running = true;
    ServerConnection sc;
};

#endif //TAGSWITCH_SWITCHMANAGER_H
