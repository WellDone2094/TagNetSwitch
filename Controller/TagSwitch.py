from threading import Thread
import subprocess
import socket
import time

class TagSwitch():

    output = ""

    def __init__(self, port):
        self.port = port;
        self.p = subprocess.Popen(["./TagSwitchMain",str(port)],
                                    stdout=subprocess.PIPE);
        self.output_t = Thread(target=TagSwitch.readOuput, args=(self,self.p,))
        self.output_t.start()
        time.sleep(1)
        self.connect()

    def readOuput(tagSwitch,proc):
        while proc.poll() is None:
            tagSwitch.output = tagSwitch.output + proc.stdout.readline().decode()

    def connect(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect(('127.0.0.1',self.port))

    def send(self, msg):
        msg = msg+'\n'
        self.s.send(msg.encode())
        return self.s.recv(2024).decode()

    def list_interfaces(self):
        return self.send('list_interfaces')

    def add_interface(self, inPort, outIp, outPort):
        msg = 'add_interface %d %d %s' %(inPort, outPort, outIp)
        return self.send(msg)

    def add_filter(self, tree, interface, filter):
        msg = 'add_filter %d %d %s' %(tree, interface, filter)
        return self.send(msg)

    def add_tags(self, tree, interface, tags):
        msg = 'add_tags %d %d' %(tree, interface)
        for tag in tags:
            msg = msg + ' '+tag
        return self.send(msg)

    def close(self):
        self.s.close()



t = TagSwitch(1236)
print(t.add_interface(4001, '127.0.0.1', 5000))
print(t.list_interfaces())
print(t.add_tags(1,2,['prova','usi','fff']))
print(t.add_filter(0,1,'0001011101100100101'))
t.close()


# while t.p.poll() is None:
#     print(t.output)
#     print(t.p.stdout.readline())
