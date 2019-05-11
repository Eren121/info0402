PROJECT = ProjectQt
CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += src/ src/utils/

HEADERS += \
    src/counter/Counter.h \
    src/map/Map.h \
    src/map/Map.tpp \
    src/map/MapIterator.tpp \
    src/map/rb_tree.h \
    src/multimap/AVLTree.h \
    src/multimap/BSTree.h \
    src/multimap/List.h \
    src/multimap/Multimap.h \
    src/multimap/MultimapIterator.h \
    src/utils/AnsiColor.h \
    src/Defines.h \
    src/catch.hpp

SOURCES += \
    src/counter/Counter.cpp \
    src/test/Algorithm.cpp \
    src/test/BSTree.cpp \
    src/test/List.cpp \
    src/test/Multimap.cpp \
    src/catch.cpp \
    src/main.cpp \
    src/map/rb_tree.c \
    src/.old-main.c

