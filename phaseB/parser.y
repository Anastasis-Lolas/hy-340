%{
#include <iostream>
#include <string>
#include "Symtable/symtable.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "expr_handler.h"


using namespace std;
extern FILE* yyin;
void yyerror (const char* yaccProvidedMessage);
extern int yylineno;
extern int yylex(void);
extern char* yytext;
extern int yylex(void);
extern unsigned int scope;
std::vector<void *> args;

#define DEBUG_REDUCE(msg) std::cout << "Reduced: " << msg << " (line " << yylineno << ")\n"
//#define DEBUG_REDUCE(msg)

%}
%code requires {
    #include "Symtable/TableEntry/SymbolTableEntry.h"
}

%start program

%union { 
	std::string *stringValue;
	int intValue ;
    SymbolTableEntry_T symEntry;
    std::vector<void *>* idList;
}


%token <intValue> INTEGER
%token IDENT 


%token IF ELSE WHILE FUNCTION FOR RETURN BREAK CONTINUE AND NOT OR NIL

%token REALCONST TRUE FALSE LOCAL

%token ASSIGN PLUS MINUS MULT DIV MOD EQUAL NOT_EQUALS PLUS_PLUS MINUS_MINUS

%token GREATER LESS GREATER_EQUAL LESS_EQUAL 

%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS

%token SEMICOLON COMMA DOUBLE_DOT DOT NAMESPACE COLON 



%token STRING
%token UNDEFINED

%type <intValue> expr
%type <symEntry> lvalue
%type <stringValue> IDENT
%type <idList> idlist

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
%right ASSIGN



%%

program:
      stmt_list     { DEBUG_REDUCE("program -> stmt_list"); }
   
    ;

stmt_list : stmt_list stmt { DEBUG_REDUCE("stmt -> expr ;"); }
            | { DEBUG_REDUCE("stmt list  -> empty ;"); }
          ;

stmt:
      expr SEMICOLON      { DEBUG_REDUCE("stmt -> expr ;"); }
    | ifstmt              { DEBUG_REDUCE("stmt -> ifstmt"); }
    | whilestmt           { DEBUG_REDUCE("stmt -> whilestmt"); }
    | forstmt             { DEBUG_REDUCE("stmt -> forstmt"); }
    | returnstmt          { DEBUG_REDUCE("stmt -> returnstmt"); }
    | BREAK SEMICOLON     { DEBUG_REDUCE("stmt -> break ;"); }
    | CONTINUE SEMICOLON  { DEBUG_REDUCE("stmt -> continue ;"); }
    | block               { DEBUG_REDUCE("stmt -> block"); }
    | funcdef             { DEBUG_REDUCE("stmt -> funcdef"); }
    | SEMICOLON           { DEBUG_REDUCE("stmt -> ;"); }
    ;




expr:
      assignexpr                              { DEBUG_REDUCE("expr -> assignexpr"); }
    | expr PLUS expr                          { DEBUG_REDUCE("expr -> expr + expr"); }
    | expr MINUS expr                         { DEBUG_REDUCE("expr -> expr - expr"); }
    | expr MULT expr                          { DEBUG_REDUCE("expr -> expr * expr"); }
    | expr DIV expr                           { DEBUG_REDUCE("expr -> expr / expr"); }
    | expr MOD expr                           { DEBUG_REDUCE("expr -> expr % expr"); }
    | expr GREATER expr                       { DEBUG_REDUCE("expr -> expr > expr"); }
    | expr GREATER_EQUAL expr                 { DEBUG_REDUCE("expr -> expr >= expr"); }
    | expr LESS expr                          { DEBUG_REDUCE("expr -> expr < expr"); }
    | expr LESS_EQUAL expr                    { DEBUG_REDUCE("expr -> expr <= expr"); }
    | expr EQUALS_EQUALS expr                 { DEBUG_REDUCE("expr -> expr == expr"); }
    | expr NOT_EQUALS expr                    { DEBUG_REDUCE("expr -> expr != expr"); }
    | expr AND expr                           { DEBUG_REDUCE("expr -> expr and expr"); }
    | expr OR expr                            { DEBUG_REDUCE("expr -> expr or expr"); }
    | term                                    { DEBUG_REDUCE("expr -> term"); }
    ;


term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
        { DEBUG_REDUCE("term -> (expr)"); }
    | MINUS expr
        { DEBUG_REDUCE("term -> -expr"); }
    | NOT expr
        { DEBUG_REDUCE("term -> not expr"); }
    | PLUS_PLUS lvalue
        {temrs_error($2,"++"); DEBUG_REDUCE("term -> ++lvalue"); }
    | lvalue PLUS_PLUS
        {temrs_error($1,"++"); DEBUG_REDUCE("term -> lvalue++"); }
    | MINUS_MINUS lvalue
        {temrs_error($2,"--"); DEBUG_REDUCE("term -> --lvalue"); }
    | lvalue MINUS_MINUS
        {temrs_error($1,"--"); DEBUG_REDUCE("term -> lvalue--"); }
    | primary
        { DEBUG_REDUCE("term -> primary"); }
    ;

assignexpr:
      lvalue ASSIGN expr
        {assign_error($1); DEBUG_REDUCE("assignexpr -> lvalue = expr"); }
    ;

primary:
      lvalue                                { DEBUG_REDUCE("primary -> lvalue");    }
    | call                                  { DEBUG_REDUCE("primary -> call");      }
    | objectdef                             { DEBUG_REDUCE("primary -> objectdef"); }
    | LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS
                                            { DEBUG_REDUCE("primary -> (funcdef)"); }                                 
    | const                                 { DEBUG_REDUCE("primary -> const");     }
    ;

lvalue:
      IDENT                                 {$$ = add_ident(*$1); DEBUG_REDUCE("lvalue -> IDENT"); }
    | LOCAL IDENT                           {$$ = add_local_dent(*$2);  DEBUG_REDUCE("lvalue -> local IDENT"); }
    | NAMESPACE IDENT                       {$$ =handle_namespace_dent(*$2);DEBUG_REDUCE("lvalue -> ::IDENT"); }
    | member                                { DEBUG_REDUCE("lvalue -> member"); }
    ;

member:
      lvalue DOT IDENT                      {member_error($1, "ident"); DEBUG_REDUCE("member -> lvalue . IDENT"); }
    | lvalue LEFT_BRACKET expr RIGHT_BRACKET    {member_error($1, "expr"); DEBUG_REDUCE("member -> lvalue [expr]"); }
    | call DOT IDENT                        { DEBUG_REDUCE("member -> call . IDENT"); }
    | call LEFT_BRACKET expr RIGHT_BRACKET      { DEBUG_REDUCE("member -> call [expr]"); }
    ;

call:
        lvalue callsuffix {null_entry($1, "function"); DEBUG_REDUCE("call -> lvalue callsuffix"); }
    | call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS { DEBUG_REDUCE("call -> call(elist)"); }
    | LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
                                           { DEBUG_REDUCE("call -> (funcdef)(elist)"); }
    ;

callsuffix:
      normcall                              { DEBUG_REDUCE("callsuffix -> normcall"); }
    | methodcall                            { DEBUG_REDUCE("callsuffix -> methodcall"); }
    ;

normcall:
      LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
                                           { DEBUG_REDUCE("normcall -> (elist)"); }
    ;

methodcall:
      NAMESPACE IDENT LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
                                           { DEBUG_REDUCE("methodcall -> ::IDENT(elist)"); }
    ;

elist:
      /* empty */                           { DEBUG_REDUCE("elist -> empty"); }
    | expr                                  { DEBUG_REDUCE("elist -> expr"); }
    | elist COMMA expr                      { DEBUG_REDUCE("elist -> elist , expr"); }
    ;

objectdef:
      LEFT_BRACKET elist RIGHT_BRACKET         { DEBUG_REDUCE("objectdef -> {elist}"); }
      | LEFT_BRACKET indexed RIGHT_BRACKET         { DEBUG_REDUCE("objectdef -> indexed "); }
    ;

indexed:
      indexedelem                           { DEBUG_REDUCE("indexed -> indexedelem"); }
    | indexed COMMA indexedelem            { DEBUG_REDUCE("indexed -> indexed , indexedelem"); }
    ;

indexedelem:
      LEFT_BRACE expr COLON expr RIGHT_BRACE
                                           { DEBUG_REDUCE("indexedelem -> [expr : expr]"); }
    ;

block:
      LEFT_BRACE { scope++; } stmt_list RIGHT_BRACE { exit_block(); DEBUG_REDUCE("block -> {stmt_list}"); }
    ;
    

funcdef:
      FUNCTION {scope++;}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS {scope--;add_function("", args);}
            block { DEBUG_REDUCE("funcdef -> function(idlist) block"); }
                                          
    | FUNCTION IDENT {scope++;}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS 
            {scope--; add_function(*$2, args);}
            block
            { DEBUG_REDUCE("funcdef -> function IDENT(idlist) block"); }
    ;


const:
      INTEGER     { DEBUG_REDUCE("const -> INTEGER"); }
    | STRING      { DEBUG_REDUCE("const -> STRING"); }
    | NIL         { DEBUG_REDUCE("const -> NIL"); }
    | TRUE        { DEBUG_REDUCE("const -> TRUE"); }
    | FALSE       { DEBUG_REDUCE("const -> FALSE"); }
    ;

idlist:
    IDENT {
        args=handle_func_args(args,*$1);
        DEBUG_REDUCE("idlist -> IDENT");
    }
  | idlist COMMA IDENT {
        args = handle_func_args(args,*$3);
        DEBUG_REDUCE("idlist -> idlist , IDENT");
    }
  | {}
;


ifstmt:
      IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
        { DEBUG_REDUCE("ifstmt -> if (expr) stmt"); }
    | IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt ELSE stmt
        { DEBUG_REDUCE("ifstmt -> if (expr) stmt else stmt"); }
    ;

whilestmt:
      WHILE LEFT_PARENTHESIS expr RIGHT_PARENTHESIS stmt
        { DEBUG_REDUCE("whilestmt -> while (expr) stmt"); }
    ;

    
forstmt:
      FOR LEFT_PARENTHESIS elist SEMICOLON expr SEMICOLON elist RIGHT_PARENTHESIS stmt
        { DEBUG_REDUCE("forstmt -> for (elist; expr; elist) stmt"); }
    ;

returnstmt:
      RETURN SEMICOLON
        { DEBUG_REDUCE("returnstmt -> return ;"); }
    | RETURN expr SEMICOLON
        { DEBUG_REDUCE("returnstmt -> return expr ;"); }
    ;
%%

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* inputFile = fopen(argv[1], "r");
        if (!inputFile) {
            perror("Failed to open file");
            return 1;
        }
        yyin = inputFile;
    }
    init_tables();

    yyparse();

    //print_args(args);
    printFullSymTable(oSymTable); 

    return 0;
}

void yyerror(const char *s) {
    cerr << "Parse error: " << s << " at line : "<<yylineno <<" before token: " <<  yytext << endl;
}
