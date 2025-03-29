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


%%
expr:
    	| expr PLUS expr	{cout << "PLUS" << endl;}
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
