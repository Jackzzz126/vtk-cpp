CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(test)

# compiler config
set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS   "-std=c++11")# c++11
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall")# 开启所有警告

set(CMAKE_BUILD_TYPE debug)
if(${CMAKE_BUILD_TYPE} MATCHES "debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG" )  # 调试包不优化
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG " )   # release包优化
endif()

# sub directory
ADD_SUBDIRECTORY(src)

