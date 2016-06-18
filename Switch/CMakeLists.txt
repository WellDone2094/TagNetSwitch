cmake_minimum_required(VERSION 3.4)
project(TagSwitch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build/Release")


set(SOURCE_FILES_MAIN
        main.cpp
        VirtualInterface.cpp VirtualInterface.h
        SyncQueue.h
        Packet.cpp Packet.h
        Switch.cpp Switch.h
        BufferManager.cpp BufferManager.h
        SwitchManager.cpp SwitchManager.h
        ServerConnection.cpp ServerConnection.h
        hashFunction.h
        patricia_predicate.cc patricia_predicate.hh
        matcher.hh
        bitvector.hh
        filter.hh
        routing.hh
        )

add_executable(TagSwitchMain ${SOURCE_FILES_MAIN})
