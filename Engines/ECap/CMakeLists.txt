cmake_minimum_required(VERSION 3.0)
message("Building ECap Engine...")
project(falcon)

#make a root and direct from there
IF(WIN32)
	set(ROOT_PATH "E:/Falcon")
ELSEIF(UNIX)
	set(ROOT_PATH "/home/pi/Falcon")	
ENDIF(WIN32)

include_directories(${ROOT_PATH}/Common/EngineFrame/inc ${ROOT_PATH}/Common/signal/inc ${ROOT_PATH}/Common/Utils/pugixml/inc ${ROOT_PATH}/Engines/ECap/inc ${ROOT_PATH}/Common/Utils/fsock/inc)

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
add_executable(ecap ./src/ecap.cpp)

set_target_properties(ecap PROPERTIES VERSION "0.3")

target_link_libraries(ecap ${ENGFRM} ${PUGI} ${SIG} ${SOCK} pthread)
