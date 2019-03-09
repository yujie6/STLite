#project name
cmake_minimum_required(VERSION 3.10)
PROJECT(vector LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
#head file path
#INCLUDE_DIRECTORIES(.)
#AUX_SOURCE_DIRECTORY(src ../data/one)
#MESSAGE("Add path successfully.")
#SET(VECTOR ${DIR_SRCS})
#SET(BIN_DIR ../bin)
#MESSAGE("Set environmental variables successfully.")
ADD_EXECUTABLE(vector ./data/one/main.cpp ./vector.hpp ./exceptions.hpp ./utility.hpp)
#TARGET_LINK_LIBRARIES(vector vector.hpp utility.hpp exceptions.hpp)
#INSTALL(TARGETS vector RUNTIME DESTINATION ${BIN_DIR} )

