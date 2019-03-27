PROJECT_NAME=Projet
CXX=g++
CC=gcc
CXXFLAGS=-Wall -Wextra -Wconversion -pedantic -ansi -Wfatal-errors -std=c++17 -g
BUILD=build
SRCDIR=src
DEPENDS=$(BUILD)/Makefile.depend
EXE=$(PROJECT_NAME)
SRC=Counter.cpp main.cpp catch_main.cpp rb_tree.c
HEADER=Counter.h Map.h Map.tpp rb_tree.h

MAKEDEPEND=./makedepend
SRC:=$(addprefix $(SRCDIR)/, $(SRC))
HEADER:=$(addprefix $(SRCDIR)/, $(HEADER))
OBJ=$(SRC:.c=.o)
OBJ:=$(OBJ:.cpp=.o)
OBJ:=$(addprefix $(BUILD)/, $(OBJ))

.PHONY: build, clean, run, depend, all

$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/%.o: %.c
	$(CC) -c -o $@ $<

build: depend $(EXE)
all: clean build

clean:
	rm -rf $(BUILD)

run: $(EXE)
	./$<

depend:
	@mkdir -p $(BUILD)/$(SRCDIR)
	@touch $(DEPENDS)
	$(MAKEDEPEND) -Y $(SRC) $(HEADER) -p $(BUILD)/ -f $(DEPENDS)

-include $(DEPENDS)
# DO NOT DELETE
