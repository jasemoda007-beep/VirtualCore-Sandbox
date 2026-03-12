cmake_minimum_required(VERSION 3.10.2)

project("VirtualCore")

add_library(VirtualCore SHARED VirtualCore.cpp)

find_library(log-lib log)

target_link_libraries(VirtualCore ${log-lib})
