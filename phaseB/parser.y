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

%type <intValue> expr

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

program : stmt_list 
        | /*empty rule */
        ;

stmt_list : stmt_list stmt {} 
          ;

stmt : expr SEMICOLON {}
     | ifstmt {}
     | whilestmt {}
     | forstmt {} 
     | returnstmt {}
     | BREAK SEMICOLON {}
     | CONTINUE SEMICOLON {}
     | block {}
     | funcdef {}
     | SEMICOLON {}

expr : assignexpr {}
     | expr op expr {}
     | term {}
     ;

op : PLUS {}
   | MINUS {}
   | MULT {}
   | DIV {}
   | MOD {}
   | GREATER {}
   | GREATER_EQUAL {}
   | LESS {}
   | LESS_EQUAL {}
   | EQUALS_EQUALS {}
   | NOT_EQUALS {}
   | AND {}
   | OR {} 
   ;

term : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {}
     | MINUS expr {}
     | NOT expr {}
     | PLUS_PLUS lvalue {}
     | lvalue PLUS_PLUS {}
     | MINUS_MINUS lvalue {}
     | DIV lvalue MINUS_MINUS {}
     | DIV primary
     ;


assignexpr : lvalue EQUALS expr {}
           ;

primary : lvalue {}
        | call {}
        | DIV LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS {}
        | DIV objectdef {} 
        | const {}
        ;

lvalue : IDENT {}
       | LOCAL IDENT {}
       | NAMESPACE IDENT {}
       | member {}
       ;

member : lvalue DOT IDENT {}
       | lvalue LEFT_BRACE expr RIGHT_BRACE {}
       | call DOT IDENT {}
       | call LEFT_BRACE expr RIGHT_BRACE {}
       ;

call : call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
     | lvalue callsuffix {}
     | LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
     ;

callsuffix : normcall {}
           | methodcall {}
           ;

normcall : LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
         ;

methodcall : IDENT LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {}
           ;

elist : elist expr_list {}
      ;

expr_list : expr COMMA expr {}
          ;

objectdef : LEFT_BRACE elist DIV indexed RIGHT_BRACE {}
          ;

indexed : indexed list_indexedelem {}
        ;

list_indexedelem : indexedelem COMMA indexedelem {}
                 ;

indexedelem : LEFT_BRACKET expr COLON expr RIGHT_BRACKET {}
            ;

block : LEFT_BRACKET stmt_list RIGHT_BRACKET {}
      | LEFT_BRACKET RIGHT_BRACKET {}
      ;

funcdef : FUNCTION IDENT LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS block
        ;

const  : INTEGER {}
       | STRING {}
       | NIL {}
       | TRUE {}
       | FALSE {} 
       ;

idlist : idlist identlist {}
       ;

identlist : IDENT COMMA IDENT {}
          ;

ifstmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {}
       | IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt {}
       ;

whilestmt : WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt {}
          ;

forstmt: FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt {}
       ;

returnstmt : RETURN SEMICOLON  {}
           | RETURN expr SEMICOLON {}
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
