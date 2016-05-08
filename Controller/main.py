from SwitchGraph import *
import configparser

switches = []
clients = []

def handler(s):
    print(s)

def parse(file):
    config = configparser.ConfigParser()
    config.read(file)
    for i in range(int(config.get('init','switch_n'))):
        if_n = int(config.get('switch%d'%(i+1), 'n_interface'))
        switches.append(SwitchNode(i+1, if_n, handler))

    for i in range(int(config.get('init','client_n'))):
        clients.append(ClientNode(i+101,getFreePort(), handler))

    for i in range(len(switches)):
        for j in range(switches[i].if_n):
            s = int(config.get('switch%d'%(i+1), str(j+1)))
            if s<0:
                switches[i].add_neighbor(clients[(-s)-1],j+1)
                clients[i].set_out(switches[i], switches[i].interfaceSwitchMap[j+1].inPort)
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

    for s in switches:
        print(s)

    for c in clients:
        print('%d: %d' %(c.id,c.inPort))

def main():
    parse('controller.cfg')
    clients[0].add_tags(0,['usi','prova'])
    clients[1].send(0,'ciaoprova',['prova'])
    clients[2].send(0,'ciaoprovausi',['prova','usi'])

if __name__ == '__main__':
    main()
