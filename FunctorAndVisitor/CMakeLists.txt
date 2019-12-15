cmake_minimum_required(VERSION 3.14)
project(Visitor)

set(CMAKE_CXX_STANDARD 11)

add_executable(Visitor main.cpp TypeList.h Functor.h VisitFactory.h Iterables.h Utils.h)