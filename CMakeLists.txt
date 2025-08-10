cmake_minimum_required(VERSION 2.8)

project(matrix_laboratory)

set(SOURCES BigFloat.cpp BigInteger.cpp matrixlab.cpp)
add_executable(matrix_laboratory ${SOURCES})
