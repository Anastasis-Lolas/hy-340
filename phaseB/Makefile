CC = g++
FLEX = flex
BISON = bison

SCANNER_L = scanner.l
LIST_CPP = List/list.cpp
SCANNER_CPP = scanner.cpp
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

$(EXEC): $(LIST_CPP) $(SCANNER_CPP) $(PARSER_CPP)
	$(CC) $^ -o $@

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
