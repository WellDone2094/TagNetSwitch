//
// Created by Andrea Benfatti on 11/04/16.
//

#ifndef TAGSWITCH_SWITCHMANAGER_H
#define TAGSWITCH_SWITCHMANAGER_H

#include <string>
#include <map>

class Switch;

enum type {
    VOID, INT, STRING, INT_STRING
};

struct SwitchMethod {
    SwitchMethod(Switch* o, void (Switch::* m)()) : s(o), t(VOID), void_m(m) {}
    SwitchMethod(Switch* o, void (Switch::* m)(int)) : s(o), t(INT), int_m(m) {}
    SwitchMethod(Switch* o, void (Switch::* m)(const std::string &)) : s(o), t(STRING), string_m(m) {}
    SwitchMethod(Switch* o, void (Switch::* m)(int, const std::string &)) : s(o), t(INT_STRING), int_string_m(m) {}

    enum type t;
    Switch *s;
    union {
        void (Switch::* string_m)(const std::string &);
        void (Switch::* int_m)(int);
        void (Switch::* void_m)();
        void (Switch::* int_string_m)(int, const std::string &);
    };
};

class SwitchManager {
public:
    void add_method(const std::string s, SwitchMethod* m);

    const std::string executeCmd(std::string);

private:
    std::map<std::string, SwitchMethod*> methodMap;
};

#endif //TAGSWITCH_SWITCHMANAGER_H
