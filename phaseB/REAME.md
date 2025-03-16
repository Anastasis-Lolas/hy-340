# Compilers Phase 1 

## Authors  
Anastasios Lolas csd5197
Alexandros Manos csd5136
Paraskevi Mourelatou csd5149

# Commands for makefile:
(1) make
(2) ./scanner < tests/inputFile

# Computer used : kerasi 

# Description
scanner.l tokenizes all types of input correctly including keywords, strings, operators, 
identifiers and comments(simple and multiline).
Tokens are stored in a list for later use.
Errors and Wanrnings related to strings are printed at the top, undefined escaped characters 
are being printed inside the string with "\" according to the example along with the warning. 
