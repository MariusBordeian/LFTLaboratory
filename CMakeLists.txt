cmake_minimum_required(VERSION 3.5)
project(LFTLaboratory)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES
    Prob1/Grammar.cpp
    Prob1/Grammar.h
    Prob1/ReaderAF.cpp
    Prob1/ReaderAF.h
    Prob1/ReaderST.cpp
    Prob1/ReaderST.h
    Prob1/RegularExpression.cpp
    Prob1/RegularExpression.h
    Prob1/Source.cpp
    Prob1/Util.cpp
    Prob1/Util.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Debug")

add_executable(LFTLaboratory ${SOURCE_FILES})

