# TagNet-Router

## Requirements

Router requirements:
* C++ compiler
* CMake
* make

Network Simulator
* python3
* pyqt-4

Network Visualizer
* python3
* networkx

## How to compile the router
* move into the Switch folder
* run `cmake ./`
* run `make`

now you should have a file called TagSwitchMain inside Switch/build/Release

## How to run the router
To run the router move inseide Switch folder and execute 
`./budld/Release 7777` 
7777 is the port where the TCP server is listening, you can connect using netcat to confiugure the router

## How to run the network visualizer
To run the network visualizer move into netVisualizer and execute `python3 netVisualizer.py`

## How to run the network simulator
To run the network simulator move into Controller and execute `python3 main`.
This command will run the network simulator as described in controller.cfg
If you run the network simualtor without the network visualizer it may not work properly


