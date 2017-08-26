cmake_minimum_required(VERSION 3.0)
message("Building DCap Engine...")
project(falcon)

#make a root and direct from there
IF(WIN32)
	set(ROOT_PATH "E:/Falcon")
ELSEIF(UNIX)
	set(ROOT_PATH "/home/pi/Falcon")	
ENDIF(WIN32)

include_directories(${ROOT_PATH}/Common/EngineFrame/inc ${ROOT_PATH}/Common/signal/inc ${ROOT_PATH}/Common/Utils/pugixml/inc ${ROOT_PATH}/Engines/DCap/inc ${ROOT_PATH}/Common/Utils/fsock/inc)

add_definitions(-DDEBUG=0)
add_compile_options(-std=c++11)

IF(WIN32)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
ENDIF(WIN32)

find_package(Threads REQUIRED)
find_library(ENGFRM EngineFrame ${ROOT_PATH}/Libraries/bin)
find_library(PUGI PugiXML ${ROOT_PATH}/Libraries/bin)
find_library(SIG signals ${ROOT_PATH}/Libraries/bin)
find_library(SOCK FSOCK ${ROOT_PATH}/Libraries/bin)
add_executable(dcap ./src/dcap.cpp)

target_link_libraries(dcap ${ENGFRM} ${PUGI} ${SIG} ${SOCK} pthread)
