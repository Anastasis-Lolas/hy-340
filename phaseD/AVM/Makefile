
CXX = g++
CXXFLAGS = -Wall -Wextra -g -w

SRC := $(wildcard *.cpp) $(wildcard execs/*.cpp) ../Symtable/symtablehash.cpp ../Symtable/TableEntry/SymbolTableEntry.cpp
OBJ := $(SRC:.cpp=.o)
TARGET = avm

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%/%.o: %/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)