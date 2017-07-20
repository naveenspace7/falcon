cmake_minimum_required(VERSION 3.0)
message("Building Falcon Libraries...")
project(falcon)

#make a root and direct from there
IF(WIN32)
	set(ROOT_PATH "E:/Falcon")
ELSEIF(UNIX)
	set(ROOT_PATH "/home/pi/Falcon")	
ENDIF(WIN32)


include_directories(${ROOT_PATH}/Common/EngineFrame/inc ${ROOT_PATH}/Common/signal/inc ${ROOT_PATH}/Common/Utils/pugixml/inc)

add_compile_options(-std=c++11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_PATH}/Libraries/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_PATH}/Libraries/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/Libraries/bin)

add_library(EngineFrame SHARED ${ROOT_PATH}/Common/EngineFrame/src/engineFrame.cpp)
add_library(PugiXML SHARED ${ROOT_PATH}/Common/Utils/pugixml/src/pugixml.cpp)
add_library(signals SHARED ${ROOT_PATH}/Common/signal/src/signals.cpp)