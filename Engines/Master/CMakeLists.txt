cmake_minimum_required(VERSION 3.0)
message("Building Master controller Engine...")
project(falcon)

#make a root and direct from there
IF(WIN32)
	set(ROOT_PATH "E:/Falcon")
ELSEIF(UNIX)
	set(ROOT_PATH "/home/pi/Falcon")	
ENDIF(WIN32)

include_directories(${ROOT_PATH}/Common/EngineFrame/inc ${ROOT_PATH}/Common/signal/inc ${ROOT_PATH}/Common/Utils/pugixml/inc ${ROOT_PATH}/Engines/Master/inc)

add_compile_options(-std=c++11)

IF(WIN32)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
ENDIF(WIN32)

find_library(ENGFRM EngineFrame ${ROOT_PATH}/Libraries/bin)
find_library(PUGI PugiXML ${ROOT_PATH}/Libraries/bin)
find_library(SIG signals ${ROOT_PATH}/Libraries/bin)

add_executable(master ./src/master.cpp)

target_link_libraries(master ${ENGFRM} ${PUGI} ${SIG})
