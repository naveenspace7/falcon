cmake_minimum_required(VERSION 3.0)
message("Building Master controller Engine...")
project(falcon)

set(ROOT_PATH "/home/pi/Falcon")

#include_directories(../../Common/EngineFrame/inc ../../Engines/Master/inc)
include_directories(${ROOT_PATH}/Common/EngineFrame/inc ${ROOT_PATH}/Common/signal/inc ${ROOT_PATH}/Common/Utils/pugixml/inc ${ROOT_PATH}/Engines/Master/inc)


add_compile_options(-std=c++11)


if(UNIX)
find_library(ENGFRM EngineFrame ${ROOT_PATH}/Libraries/bin)
find_library(PUGI PugiXML ${ROOT_PATH}/Libraries/bin)
find_library(SIG signals ${ROOT_PATH}/Libraries/bin)
#link_directories(${ROOT_PATH}/Libraries/bin)
add_executable(master ./src/master.cpp)
endif(UNIX)

#find_library(ENGFRM EngineFrame) #HINTS ${ROOT_PATH}/Libraries/bin)
#find_package(EngineFrame)
#find_package(PugiXML)
#find_package(signals)

#target_link_libraries(arduino ${ROOT_PATH}/Libraries/bin/EngineFrame ${ROOT_PATH}/Libraries/bin/PugiXML ${ROOT_PATH}/Libraries/bin/signals)

target_link_libraries(master ${ENGFRM} ${PUGI} ${SIG})
