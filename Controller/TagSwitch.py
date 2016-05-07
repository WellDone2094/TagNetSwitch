from threading import Thread
import subprocess
import socket
import time

class TagSwitch():
    def __init__(self, port, outputHandler):
        self.port = port
        self.interfaces = []
        self.outputHandler = outputHandler

        self.p = subprocess.Popen(["../Switch/Build/Release/TagSwitchMain",str(port)],
                                    stdout=subprocess.PIPE);
        self.output_t = Thread(target=TagSwitch.readOuput, args=(self, self.p, self.outputHandler))
        self.output_t.start()
        time.sleep(1)
        self.connect()

    def readOuput(tagSwitch,proc, outputHandler):
        while proc.poll() is None:
            outputHandler(proc.stdout.readline().decode())

    def connect(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.connect(('127.0.0.1',self.port))

    def send(self, msg):
        msg = msg+'\n'
        self.s.send(msg.encode())
        return self.s.recv(2024).decode()

    def list_interfaces(self):
        return self.send('list_interfaces')

    def add_interface(self, inPort):
        msg = 'add_interface %d' %(inPort)
        return self.send(msg)

    def set_interface_out(self, id, port, ip):
        msg = 'set_interface_out %d %d %s' %(id, port, ip)
        return self.send(msg)

    def add_filter(self, tree, interface, filter):
        msg = 'add_filter %d %d %s' %(tree, interface, filter)
        return self.send(msg    )

    def add_tags(self, tree, interface, tags):
        msg = 'add_tags %d %d' %(tree, interface)
        for tag in tags:
            msg = msg + ' '+tag
        return self.send(msg)

    def start_interface(self, interface):
        msg = 'start_interface %d' %interface
        return self.send(msg)

    def stop_interface(self, interface):
        msg = 'stop_interface %d' %interface
        return self.send(msg)

    def close(self):
        msg = 'quit'
        resp = self.send(msg)
        self.output_t.join()
        return resp


# def handler(s):
    # print(s)


# t = TagSwitch(1235, handler)
# print(t.add_interface(4000))
# print(t.set_interface_out(1,5000,'127.0.0.1'))
# print(t.close())
