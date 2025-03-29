%{
#include <iostream>
using namespace std;

void yyerror (const char* yaccProvidedMessage);
extern int yylineno;
extern FILE* yyout;
extern int yylex(void);
extern char* yytext;
extern int yylex(void);

%}

%token INTEGER 
%token PLUS
%token IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE AND NOT OR LOCAL
%token TRUE FALSE NIL REALCONST IDENT ASSIGN MINUS MULT DIV MOD EQUAL
%token NOT_EQUALS PLUS_PLUS MINUS_MINUS GREATER LESS GREATER_EQUAL LESS_EQUAL
%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET SEMICOLON
%token LEFT_PARENTHESIS RIGHT_PARENTHESIS COMMA DOUBLE_DOT DOT NAMESPACE COLON
%token BLOCK_COMMENT NESTED_COMMENT LINE_COMMENT STRING UNDEFINED


%%
expr:
    	| expr PLUS expr	{cout << "PLUS " << $1 + $3 <<  endl;}
		| term			{cout << "term!" << endl;}
		;
term:
    	INTEGER		{ cout << "integer: " << $1 << endl;}
	;

%%

int main() {
   yyout = stdout; 
   yyparse();
   return 0;
}

void yyerror(const char *s) {
    cerr << "Parse error: " << s << endl;
}
