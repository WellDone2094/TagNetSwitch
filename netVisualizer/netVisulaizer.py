import tkinter as tk
import networkx as nx
import time
import socket
# import matplotlib.pyplot as plt
from PIL import Image, ImageTk
from threading import *

condition = Condition()
queue = []
UDP_IP = "127.0.0.1"
UDP_PORT = 5006

def calculatePosition(graph, width, height, ofx, ofy):
    nodes = set([n1 for n1, n2 in graph] + [n2 for n1, n2 in graph])

    # create networkx graph
    G=nx.Graph()
    # add nodes
    for node in nodes:
        G.add_node(node)
    # add edges
    for edge in graph:
        G.add_edge(edge[0], edge[1])
    # draw graph
    pos = nx.nx_agraph.pygraphviz_layout(G)
    return normalize(pos, width, height, ofx, ofy)


class Executer(Thread):
    def __init__(self, app):
        self.app = app
        self.graph = []
        super(Executer, self).__init__()

    def run(self):
        global queue
        while True:
            condition.acquire()
            if not queue:
                condition.wait()
            line = queue.pop(0)
            words = line.split(' ')
            try:
                if words[0].strip() == 'message':
                    self.app.message(words[1].strip(), words[2].strip())
                if words[0].strip() == 'connect':
                    self.graph.append((words[1].strip(), words[2].strip()))
                if words[0].strip() == 'draw':
                    self.app.draw(self.graph)
                if words[0].strip() == 'reset':
                    self.graph = []
                    self.app.reset()
            except:
                print ('error')
            condition.release()

class UDPServer(Thread):
    def __init__(self):
        global UDP_IP
        global UDP_PORT
        self.sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM)
        self.sock.bind((UDP_IP, UDP_PORT))
        super(UDPServer, self).__init__()

    def run(self):
        global queue
        while True:
            data, addr = self.sock.recvfrom(1024) # buffer size is 1024 bytes
            condition.acquire()
            queue = queue + data.decode().split('\n')
            condition.notify()
            condition.release()

def normalize(nodes, width, height, ofx, ofy):
    minx = miny = float("inf")
    maxx = maxy = float("-inf")
    for k in nodes:
        minx = minx if minx < nodes[k][0] else nodes[k][0]
        maxx = maxx if maxx > nodes[k][0] else nodes[k][0]
        miny = miny if miny < nodes[k][1] else nodes[k][1]
        maxy = maxy if maxy > nodes[k][1] else nodes[k][1]

    xnf = maxx - minx
    ynf = maxy - miny

    for k in nodes:
        nodes[k] = ((nodes[k][0] - minx)/xnf*width + ofx,(nodes[k][1] - miny)/ynf*height + ofy)

    return nodes


class SampleApp(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        # create a canvas
        self.canvas = tk.Canvas(width=600, height=600)
        self.canvas.pack(fill="both", expand=True)

        self.router = ImageTk.PhotoImage(Image.open("router.jpg"))
        self.client = ImageTk.PhotoImage(Image.open("client.jpg"))
        self.msg = ImageTk.PhotoImage(Image.open("msg.jpg"))
        self.objects = []

    def reset(self):
        for id in self.objects:
            self.canvas.delete(id)
        self.object = []
        self.connections = []
        self.nodes = {}
        self.canvas.update_idletasks()

    def draw(self, connections):
        self.connections = connections
        self.nodes = calculatePosition(connections, 500, 500, 50, 50)
        print(self.connections)
        print(self.nodes)
        for id in self.objects:
            self.canvas.delete(id)

        for c in self.connections:
            id = self.canvas.create_line(self.nodes[c[0]][0], self.nodes[c[0]][1], self.nodes[c[1]][0], self.nodes[c[1]][1], fill='black')
            self.objects.append(id)
        for k in self.nodes:
            if k[0] == 's':
                id = self.canvas.create_image(self.nodes[k][0], self.nodes[k][1], image=self.router)
            else:
                id = self.canvas.create_image(self.nodes[k][0], self.nodes[k][1], image=self.client)
            textId = self.canvas.create_text(self.nodes[k][0], self.nodes[k][1], text=k)
            self.objects.append(id)
            self.objects.append(textId)
        self.canvas.update_idletasks()

    def message(self, fro, to):
        msg = self.canvas.create_image(self.nodes[fro][0], self.nodes[fro][1], image=self.msg)
        xdelta = self.nodes[to][0] - self.nodes[fro][0]
        ydelta = self.nodes[to][1] - self.nodes[fro][1]
        for i in range(30):
            self.canvas.move(msg, xdelta/30, ydelta/30)
            self.canvas.update_idletasks()
            time.sleep(1/30)

        self.canvas.delete(msg)



def sendMessages(app):
    time.sleep(1)
    app.message('c1','s1')
    app.message('s1','s2')
    app.message('s2','c2')


if __name__ == "__main__":
    # graph = [(20, 21),(21, 22),(22, 23), (23, 24),(24, 25), (25, 20),(2, 3),(3, 4),(12, 13)]
    # graph = [('c1','c2'),('c1','c3'),('c3','c4'),('c1','c6'),('c1','c7'),('c1','c8'),('c2','c8'),('c2','c7')]
    graph = [('s1','s2'),('s1','s3'),('s3','s4'),('s1','s4'),('c1','s1'),('c2','s2'),('c3','s3'),('c4','s4')]#,('t1','t2'),('t1','t3'),('t3','t4'),('t1','t4'),('c1','t1'),('c2','t2'),('c3','t3'),('c4','t4')]
    nodes = set([n1 for n1, n2 in graph] + [n2 for n1, n2 in graph])

    # create networkx graph
    G=nx.Graph()
    # add nodes
    for node in nodes:
        G.add_node(node)
    # add edges
    for edge in graph:
        G.add_edge(edge[0], edge[1])
    # draw graph
    pos = nx.nx_agraph.pygraphviz_layout(G)
    print(pos)


    app = SampleApp()
    server = UDPServer();
    executer = Executer(app)
    server.start()
    executer.start()
    # app.draw(normalize(pos),graph)
    # app.draw(normalize(pos),graph)
    app.mainloop()
