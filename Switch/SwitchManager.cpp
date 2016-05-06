//
// Created by Andrea Benfatti on 11/04/16.
//

#include <sstream>
#include <zconf.h>
#include "SwitchManager.h"
#include "Switch.h"
#include "ServerConnection.h"

void SwitchManager::TCPServer(){
    int nbyte = 1;
    int clientfd;
    char buff[1024];
    std::string str = "";
    while(running){
        clientfd = sc.takeConn();
        nbyte = recv(clientfd, buff, 1024, 0);

        while(nbyte>0 && running){
            str.append(buff, nbyte);
            std::stringstream ss(str);
            std::string line;
            std::string resp;
            while (std::getline(ss, line)) {
                if(!ss.eof()) {
                    resp = executeCmd(line);
                    std::cout << resp << std::endl;
                    send(clientfd, resp.c_str(), resp.length(), 0);
                    str="";
                }else{
                    str = line;
                }
            }
            if(!running)
                close(clientfd);
            nbyte = recv(clientfd, buff, 10, 0);
        }
        close(clientfd);
    }
}

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
    std::cout << methodMap[cmd] << std::endl;
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
                return "invalid argument\n";
            }
            break;
        case INT_STRING: iss >> s1 >> s2;
            try {
                n1 = std::stoi(s1, nullptr);
                return (sw->*methodMap[cmd]->int_string_m)(n1, s2);
            }catch (const std::invalid_argument& e){
                return "invalid argument\n";
            }
            break;
        case INT_INT_STRING: iss >> s1 >> s2 >> s3;
            try {
                n1 = std::stoi(s1, nullptr);
                n2 = std::stoi(s2, nullptr);
                return (sw->*methodMap[cmd]->int_int_string_m)(n1,n2, s3);
            }catch (const std::invalid_argument& e){
                return "invalid argument\n";
            }
            break;
        case INT_INT_LIST: iss >> s1 >> s2;
            try {
                n1 = std::stoi(s1, nullptr);
                n2 = std::stoi(s2, nullptr);
                std::vector<std::string> v;
                while(iss >> s3){
                    v.push_back(s3);
                }
                return (sw->*methodMap[cmd]->int_int_list_m)(n1, n2, v);
            }catch (const std::invalid_argument& e){
                return "invalid argument\n";
            }

    }
}

void SwitchManager::add_method(const std::string s, SwitchMethod* m) {
    methodMap[s] = m;
}

void SwitchManager::quit() {
    running = false;
}
