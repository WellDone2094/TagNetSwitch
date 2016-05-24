import socket
import TagSwitch
from TagSwitch import *
from BitVector import *
from HashFunction import *
from gui.mainwindow import *
import sys


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
        self.inPort = port
        self.outPort = -1







####################################

############ ClientNode ############

####################################
def h():
    print('h')

class ClientNode:
    def __init__(self, id, port, handler):
        self.id = id
        self.inPort = port
        self.outPort = -1
        self.switch = None
        self.running = True
        self.sSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.cSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sSock.bind(('127.0.0.1',self.inPort))
        self.outputHandler = h
        self.handler = handler

        self.setupGui()
        self.server = Thread(target=ClientNode.clientServer, args=(self, self.ui.received))
        self.server.start()

    def setupGui(self):
        self.window = QtGui.QMainWindow()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self.window,self)
        self.window.show()

    def get_interface_port(self, sw):
        return self.inPort

    def set_out(self, switch, port):
        self.switch = switch
        self.outPort = port

    def clientServer(client, handler):
        while client.running:
            data = client.sSock.recv(1024)
            handler(data[24:].decode())

    def add_tags(self, tree, tags):
        self.switch.add_tags(tree, tags, self)

    def send(self, tree, msg, tags):
            bv = BitVector(192)
            for t in tags:
                bv.setTag(t)
            if tree < 256:
                btree = bytes([0,tree])
            elif tree > 65535:
                tree = 65535
                btree = bytes([255,255])
            else:
                btree = bytes([tree>>8, tree&2555])
            bmsg = btree + bv.getBytes() + msg.encode()
            self.handler('packet_sent %d %d' %(self.inPort, self.outPort))
            self.cSock.sendto(bmsg, ('127.0.0.1',self.outPort))








####################################

############ SwitchNode ############

####################################
class SwitchNode:

    def __init__(self,id, if_n, handler, portSwitchMap):
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
            portSwitchMap[p] = 's%d' %id
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
        return self.switchInterfaceMap[sw].inPort

    def get_connected_port(self, i):
        return self.interfaceSwitchMap[i].switch.get_interface_port(self)

    def connectSwitch(self):
        for i in range(1, self.if_n+1):
            if self.interfaceSwitchMap[i].switch == None: continue
            self.interfaceSwitchMap[i].outPort = self.get_connected_port(i)
            print(self.tagSwitch.set_interface_out(i, self.get_connected_port(i),'127.0.0.1'))


    def add_tags(self, tree, tags, sw):
        # add rules
        self.tagSwitch.add_tags(tree, self.switchInterfaceMap[sw].id, tags)
        for s in self.trees[tree]:
            if s != sw:
                s.add_tags(tree, tags, self)

    def reset_filters(self):
        print(self.tagSwitch.reset_filters())

    def __str__(self):
        s = "switch%d" %self.id
        for i in self.interfaceSwitchMap:
            if(self.interfaceSwitchMap[i].switch == None):
                s = s+'\n\t%d: None' %i
            else:
                s = s+'\n\t%d: %d\tportIn:%d\tportOut:%d' %(i, self.interfaceSwitchMap[i].switch.id, self.interfaceSwitchMap[i].inPort, self.interfaceSwitchMap[i].outPort)
        return s
