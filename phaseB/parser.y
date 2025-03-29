%{
#include <iostream>
#include <string>
using namespace std;

void yyerror (const char* yaccProvidedMessage);
extern int yylineno;
extern FILE* yyout;
extern int yylex(void);
extern char* yytext;
extern int yylex(void);


%}

%start program

%union { 
	std::string *stringValue;
	int intValue ;
}


%token <intValue> INTEGER
%token <stringValue> IDENT 


%token IF
%token ELSE
%token WHILE
%token FOR
%token FUNCTION
%token RETURN
%token BREAK
%token CONTINUE
%token AND
%token NOT
%token OR
%token LOCAL
%token TRUE
%token FALSE
%token NIL
%token REALCONST
%token ASSIGN
%token PLUS
%token MINUS
%token MULT
%token DIV
%token MOD
%token EQUAL
%token NOT_EQUALS
%token PLUS_PLUS
%token MINUS_MINUS
%token GREATER
%token LESS
%token GREATER_EQUAL
%token LESS_EQUAL
%token LEFT_BRACE
%token RIGHT_BRACE
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token SEMICOLON
%token LEFT_PARENTHESIS
%token RIGHT_PARENTHESIS
%token COMMA
%token DOUBLE_DOT
%token DOT
%token NAMESPACE
%token COLON
%token BLOCK_COMMENT
%token NESTED_COMMENT
%token LINE_COMMENT
%token STRING
%token UNDEFINED

%left LEFT_PARENTHESIS RIGHT_PARENTHESIS
%left LEFT_BRACE RIGHT_BRACE 
%left DOT DOUBLE_DOT
%right NOT PLUS_PLUS MINUS_MINUS 
%left MULT DIV  MOD 
%right PLUS MINUS
%nonassoc GREATER GREATER_EQUAL LESS LESS_EQUAL
%nonassoc NOT_EQUALS EQUALS_EQUALS
%left AND
%left OR 
%right EQUALS



%%

program:
    assignments exprs
    | /* empty */
    ;

expr:
    INTEGER
    | IDENT {std::cout << "IDENT -> " <<*$1<< std::endl;}
    | expr PLUS expr {std::cout << "New addition!"<<std::endl;}
    | expr MINUS expr
    | expr MULT expr
    | expr DIV expr
    ;

expr_line:
    expr '\n'
    ;

exprs:
    exprs expr_line
    | expr_line
    ;

assignment:
    IDENT EQUALS expr 
    ;

assignments:
    assignments assignment
    | /* empty */
    ;
%%



int main() {
   yyout = stdout; 
   yyparse();
   return 0;
}

void yyerror(const char *s) {
    cerr << "Parse error: " << s << " at line : "<<yylineno <<" before token : " <<  yytext<< endl;
}
