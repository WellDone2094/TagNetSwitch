import random
import configparser
import socket
import TagSwitch
from TagSwitch import *

def handler(s):
    print(s)

def getFreePort():
    s = socket.socket()
    s.bind(('',0))
    port = s.getsockname()[1]
    s.close()
    return port


class Interface:
    def __init__(self, id, port):
        self.switch = None
        self.id = id
        self.port = port

class ClientNode:
    def __init__(self, id, port):
        self.id = id
        self.inPort = port
        self.outPort = -1
        self.running = true
        self.sSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.cSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sSock.bind(('127.0.0.1',port))

    def get_interface_port(self, sw):
        return self.inPort

    def set_out(self, port):
        self.outPort = port

    def clientServer(client, handler):
        while client.running:
            data = client.sSock.recv(1024)
            handler(data)

    def send(self, msg, tags):
            self.cSock.sendto()




class SwitchNode:

    def __init__(self,id,if_n):
        self.tagSwitch = TagSwitch(getFreePort(), handler)
        self.ip = '127.0.0.1'
        self.interfaceSwitchMap = {}
        self.switchInterfaceMap = {}
        self.trees = {}
        self.neighbors = []
        self.id = id
        self.if_n = if_n
        for i in range(if_n):
            p = getFreePort()
            self.interfaceSwitchMap[i+1] = Interface(i+1, p)
            print(self.tagSwitch.add_interface(p))

    def add_neighbor(self, sn, i):
        self.interfaceSwitchMap[i].switch = sn
        self.switchInterfaceMap[sn] = self.interfaceSwitchMap[i]

    def add_tree_neighbor(self, id, sn):
        if id not in self.trees.keys():
            self.trees[id] = []
        self.trees[id].append(sn)

    def get_interface_port(self, sw):
        return self.switchInterfaceMap[sw].port

    def get_connected_port(self, i):
        return self.interfaceSwitchMap[i].switch.get_interface_port(self)

    def connectSwitch(self):
        for i in range(1, self.if_n+1):
            if self.interfaceSwitchMap[i].switch == None: continue
            print(self.tagSwitch.set_interface_out(i, self.get_connected_port(i),'127.0.0.1'))


    def add_tags(self, tree, tags, sw):
        # add rules
        print("tags: %s, to switch: %d" %(tags, sw.id))
        for s in self.trees[tree]:
            if s != sw:
                s.add_tags(tree, tags, self)

    def __str__(self):
        s = "switch%d" %self.id
        for i in self.interfaceSwitchMap:
            if(self.interfaceSwitchMap[i].switch == None):
                s = s+'\n\t%d: None' %i
            else:
                s = s+'\n\t%d: %d\tport:%d' %(i, self.interfaceSwitchMap[i].switch.id, self.interfaceSwitchMap[i].port)
        return s


config = configparser.ConfigParser()
config.read('controller.cfg')

switches = []
clients = []

######## parser begin  ########

for i in range(int(config.get('init','switch_n'))):
    if_n = int(config.get('switch%d'%(i+1), 'n_interface'))
    switches.append(SwitchNode(i+1, if_n))

for i in range(int(config.get('init','client_n'))):
    clients.append(ClientNode(i+101,getFreePort()))

for i in range(len(switches)):
    for j in range(switches[i].if_n):
        s = int(config.get('switch%d'%(i+1), str(j+1)))
        if s<0:
            switches[i].add_neighbor(clients[(-s)-1],j+1)
        else:
            switches[i].add_neighbor(switches[s-1],j+1)

for i in range(int(config.get('init','tree_n'))):
    for j in range(len(switches)):
        sl = config.get('tree%d'%(i+1), str(j+1)).split(',')
        for n in sl:
            sn = int(n)
            switches[j].add_tree_neighbor(i,switches[sn-1])



for s in switches:
    s.connectSwitch()
    print(s)
######## parser end  ########

switches[0].add_tags(0,"prova",clients[0])
