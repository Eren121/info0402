cmake_minimum_required(VERSION 3.10)
project(QtProject)

set(INCLUDEPATH src/ src/utils/)

set(HEADERS
    src/counter/Counter.h
    src/multimap/AVLTree.h
    src/multimap/BSTree.h
    src/multimap/List.h
    src/multimap/Multimap.h
    src/multimap/MultimapIterator.h
    src/multimap/Traits.h
    src/utils/AnsiColor.h
    src/Defines.h
    src/catch.hpp)

set(SOURCES
    src/counter/Counter.cpp
    src/test/Algorithm.cpp
    src/test/BSTree.cpp
    src/test/List.cpp
    src/test/TestLookup.cpp
    src/test/TestValueCompare.cpp
    src/test/TestConstructor.cpp
    src/test/TestIterator.cpp
	src/test/TestModifier.cpp
	src/test/TestMemory.cpp
    src/catch.cpp
    src/main.cpp)

include_directories(${INCLUDEPATH})
add_executable(QtProject ${SOURCES} ${HEADERS})
