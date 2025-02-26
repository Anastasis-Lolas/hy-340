CC = g++
FLEX = flex

# Ta vazw etsi, gia na ginoun eukola remove otan tha vgaloume lista klp
SCANNER_L = scanner.l
LIST_CPP = List/list.cpp
SCANNER_CPP = scanner.cpp
EXEC = scanner

all: $(EXEC)

$(SCANNER_CPP): $(SCANNER_L)
	$(FLEX) -o $@ $<

$(EXEC): $(LIST_CPP) $(SCANNER_CPP)
	$(CC) $^ -o $@

ComTest: $(EXEC)
	cd tests && python3 checker.py ../scanner comment.in comment.out
	
clean:
	rm -f $(SCANNER_CPP) $(EXEC)
