CC = g++
CXXFLAGS = -std=c++17
FLEX = flex
BISON = bison

SCANNER_L = scanner.l
LIST_CPP = List/list.cpp
SYMTABLE_CPP = Symtable/symtablehash.cpp 
SYM_ENTRY_CPP = Symtable/TableEntry/SymbolTableEntry.cpp
SCOPE_LIST_CPP = Symtable/ScopeList/scopelist.cpp
HANDLER_CPP = expr_handler.cpp
SCANNER_CPP = scanner.cpp
SCOPE_CPP = Symtable/ScopeList/scopelist.cpp
PARSER_Y = parser.y
PARSER_CPP = parser.cpp
PARSER_H = parser.hpp

EXEC = scanner

TESTS_DIR = tests
BUILD_DIR = .
# yacc files
SRC_FILES := $(shell find . -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \))

all: $(EXEC)

$(SCANNER_CPP): $(SCANNER_L) $(PARSER_H)
	$(FLEX) -o $@ $<

$(PARSER_CPP) $(PARSER_H): $(PARSER_Y)
	$(BISON) -d -v -o $(PARSER_CPP) $<

$(EXEC): $(LIST_CPP) $(SYMTABLE_CPP) $(SCOPE_CPP) $(SYM_ENTRY_CPP) $(SCANNER_CPP) $(PARSER_CPP)  $(HANDLER_CPP)  # Added SYMTABLE_CPP
	$(CC) $(CXXFLAGS) $^ -o $@

handler: $(LIST_CPP) $(SYMTABLE_CPP) $(SCOPE_LIST_CPP) $(SYM_ENTRY_CPP) $(HANDLER_CPP)
	$(CC) $(CXXFLAGS) $^ -o $@
test: $(EXEC)
	cd tests && python3 checker.py ../scanner $(FILE).in $(FILE).out

format:
	clang-format -style=file -i $(SRC_FILES)

memcheck: $(BUILD_DIR)/scanner
	@valgrind --leak-check=full \
	         --show-leak-kinds=definite,indirect,possible,reachable \
	         --track-origins=yes \
	         --verbose \
	         --log-file=memcheck.out \
	         $(BUILD_DIR)/scanner $(TESTS_DIR)/operator1.in > /dev/null
	@printf "\n-> Valgrind memcheck log file (memcheck.out) generated\n\n"

clean:
	rm -f $(SCANNER_CPP) $(PARSER_CPP) $(PARSER_H) $(EXEC)
