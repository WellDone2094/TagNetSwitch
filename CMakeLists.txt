cmake_minimum_required(VERSION 3.4)
project(TagSwitch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES_SERVER udp_server_test.cpp)
#add_executable(TagSwitchServer ${SOURCE_FILES_SERVER})
#
set(SOURCE_FILES_CLIENT test/udp_client_test.cpp)
add_executable(TagSwitchClient ${SOURCE_FILES_CLIENT})

set(SOURCE_FILES_MAIN main.cpp VirtualInterface.cpp VirtualInterface.h SyncQueue.h Packet.cpp Packet.h Switch.cpp Switch.h BufferManager.cpp BufferManager.h SwitchManager.cpp SwitchManager.h)
add_executable(TagSwitchMain ${SOURCE_FILES_MAIN})
