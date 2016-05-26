from SwitchGraph import *
import configparser
import time
import sys
from gui.mainwindow import *
from threading import Semaphore
import socket

switches = []
clients = []
UDP_IP = "127.0.0.1"
UDP_PORT = 5006
sem = Semaphore()
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
portSwitchMap = {}

def reset_filters():
    for s in switches:
        s.reset_filters()
    for c in clients:
        s.reset_filters()

def handler(msg):
    sem.acquire()
    words = msg.split(' ')
    if words[0] == 'packet_sent':
        print(msg)
        try:
            msg = 'message %s %s' %(portSwitchMap[int(words[1].strip())],portSwitchMap[int(words[2].strip())])
            sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))
        except ValueError:
            print('cant convert')
    sem.release()

def parse(file):
    config = configparser.ConfigParser()
    config.read(file)

    for i in range(int(config.get('init','switch_n'))):
        if_n = int(config.get('switch%d'%(i+1), 'n_interface'))
        switches.append(SwitchNode(i+1, if_n, handler, portSwitchMap))

    for i in range(int(config.get('init','client_n'))):
        port = getFreePort()
        portSwitchMap[port] = 'c%d' %(i+1)
        clients.append(ClientNode(i+1,port, handler, clients, switches))

    print(portSwitchMap)

    msg = 'reset'
    sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))
    for i in range(len(switches)):
        for j in range(switches[i].if_n):
            s = int(config.get('switch%d'%(i+1), str(j+1)))
            if s<0:
                switches[i].add_neighbor(clients[(-s)-1],j+1)
                clients[i].set_out(switches[i], switches[i].interfaceSwitchMap[j+1].inPort)
                msg = 'connect s%d c%d' %(i+1,-s)
                sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))
            else:
                if s > (i+1):
                    msg = 'connect s%d s%d' %(i+1,s)
                    print(msg)
                    sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))
                switches[i].add_neighbor(switches[s-1],j+1)

    msg = 'draw'
    sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))


    for i in range(int(config.get('init','tree_n'))):
        for j in range(len(switches)):
            sl = config.get('tree%d'%(i+1), str(j+1)).split(',')
            for n in sl:
                sn = int(n)
                switches[j].add_tree_neighbor(i,switches[sn-1])



    for s in switches:
        s.connectSwitch()

    for s in switches:
        print(s)

    for c in clients:
        print('%d: %d' %(c.id,c.inPort))

def main():
    parse('controller.cfg')
    clients[0].add_tags(0,['usi'])
    switches[0].reset_filters()
    # time.sleep(2)
    # switches[0].reset_filters()
    # switches[1].reset_filters()
    # time.sleep(2)
    # clients[2].send(0,'aaaaaaaaaaaaaaaaaaaaa',['usi'])

if __name__ == '__main__':
    import sys
    app = QtGui.QApplication(sys.argv)
    main()
    sys.exit(app.exec_())
