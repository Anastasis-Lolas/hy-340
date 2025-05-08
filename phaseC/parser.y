%{
#include <iostream>
#include <string>
#include "Symtable/symtable.h"
#include "Symtable/TableEntry/SymbolTableEntry.h"
#include "Symtable/ScopeList/scopelist.h"

#include "Quads/quad.h"
#include "expr_handler.h"




using namespace         std;
extern FILE*            yyin;
void yyerror            (const char* yaccProvidedMessage);
extern int              yylineno;
extern int              yylex(void);
extern char*            yytext;
extern int              yylex(void);
extern unsigned int     scope;
extern unsigned int     currQuad;
std::vector<void *>     args;


//#define DEBUG_REDUCE(msg) std::cout << "Reduced: " << msg << " (line " << yylineno << ")\n"
#define DEBUG_REDUCE(msg)

%}
%code requires {
    #include "Symtable/TableEntry/SymbolTableEntry.h"
    #include "Quads/expression.h"
    #include "Quads/quad.h"
    #include "expr_handler.h"
}

%start program

%union { 
	std::string *stringValue;
	int intValue ;
    double doubleVal;
    SymbolTableEntry_T symEntry;
    std::vector<void *>* idList;
    expr* exprVal;
    stmt_t * s;
    forloop_t * loop_t;
}


%token <intValue> INTEGER
%token <doubleVal> DOUBLE REALCONST
%token IDENT 



%token IF ELSE WHILE FUNCTION FOR RETURN BREAK CONTINUE AND NOT OR NIL

%token  TRUE FALSE LOCAL

%token ASSIGN PLUS MINUS MULT DIV MOD EQUAL NOT_EQUALS PLUS_PLUS MINUS_MINUS

%token GREATER LESS GREATER_EQUAL LESS_EQUAL 

%token LEFT_BRACE RIGHT_BRACE LEFT_BRACKET RIGHT_BRACKET LEFT_PARENTHESIS RIGHT_PARENTHESIS

%token SEMICOLON COMMA DOUBLE_DOT DOT NAMESPACE COLON 



%token <stringValue> STRING
%token UNDEFINED

%type <s> stmt
%type <exprVal> member assignexpr term primary 
%type <exprVal> expr call const
%type <exprVal> lvalue
%type <stringValue> IDENT
%type <idList> idlist
%type <exprVal> ifprefix elseprefix ifstmt whilestart whilecond  whilestmt M N 
%type <loop_t> forprefix

%left LEFT_PARENTHESIS RIGHT_PARENTHESIS
%left LEFT_BRACE RIGHT_BRACE 
%left DOT DOUBLE_DOT
%right NOT PLUS_PLUS MINUS_MINUS 
%left MULT DIV  MOD 
%right PLUS MINUS
%nonassoc GREATER GREATER_EQUAL LESS LESS_EQUAL
%nonassoc NOT_EQUALS EQUAL
%left AND
%left OR 
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
    | expr PLUS expr                          {  DEBUG_REDUCE("expr -> expr + expr");  $$ = emit_arith_op(add, $1, $3); }
    | expr MINUS expr                         { DEBUG_REDUCE("expr -> expr - expr");   $$ = emit_arith_op(sub, $1, $3); }
    | expr MULT expr                          { DEBUG_REDUCE("expr -> expr * expr");   $$ = emit_arith_op(mul, $1, $3); }
    | expr DIV expr                           { DEBUG_REDUCE("expr -> expr / expr");   $$ = emit_arith_op(divv, $1, $3); }
    | expr MOD expr                           { DEBUG_REDUCE("expr -> expr % expr");   $$ = emit_arith_op(mod, $1, $3); }
    | expr GREATER expr                       { DEBUG_REDUCE("expr -> expr > expr");   $$ = newexpr(boolexpr_e);
		                                        $$->sym = newtemp();
		                                        emit(if_greater,  $1, $3,$$, nextquad()+3 , yylineno);
		                                        emit(assign, newexpr_bool(false), NULL,$$, -1 , yylineno);
		                                        emit(jump,NULL,NULL,NULL,nextquad()+2, yylineno);}
    | expr GREATER_EQUAL expr                 { DEBUG_REDUCE("expr -> expr >= expr");   $$ = newexpr(boolexpr_e);
		                                        $$->sym = newtemp();
		                                        emit(if_greatereq,  $1, $3,$$, nextquad()+3 , yylineno);
		                                        emit(assign, newexpr_bool(false), NULL,$$, -1 , yylineno);
		                                        emit(jump,NULL,NULL,NULL,nextquad()+2, yylineno);}
    | expr LESS expr                          { DEBUG_REDUCE("expr -> expr < expr");   $$ = newexpr(boolexpr_e);
		                                        $$->sym = newtemp();
		                                        emit(if_less,  $1, $3,$$, nextquad()+3 , yylineno);
		                                        emit(assign, newexpr_bool(false), NULL,$$, -1 , yylineno);
		                                        emit(jump,NULL,NULL,NULL,nextquad()+2, yylineno);}
    | expr LESS_EQUAL expr                    { DEBUG_REDUCE("expr -> expr <= expr");  
                                                $$ = newexpr(boolexpr_e);
		                                        $$->sym = newtemp();
		                                        emit(if_lesseq,  $1, $3,$$, nextquad()+3 , yylineno);
		                                        emit(assign, newexpr_bool(false), NULL,$$, -1 , yylineno);
		                                        emit(jump,NULL,NULL,NULL,nextquad()+2, yylineno);
		                                        emit(assign, newexpr_bool(true), NULL,$$ ,-1 , yylineno);}
    | expr EQUAL expr                         { DEBUG_REDUCE("expr -> expr == expr");  $$ = newexpr(boolexpr_e);
		                                        $$->sym = newtemp();
		                                        emit(if_eq,  $1, $3,$$, nextquad()+3 , yylineno);
		                                        emit(assign, newexpr_bool(false), NULL,$$, -1 , yylineno);
		                                        emit(jump,NULL,NULL,NULL,nextquad()+2, yylineno);}
    | expr NOT_EQUALS expr                    { DEBUG_REDUCE("expr -> expr != expr");   $$ = newexpr(boolexpr_e);
		                                        $$->sym = newtemp();
		                                        emit(if_noteq,  $1, $3,$$, nextquad()+3 , yylineno);
		                                        emit(assign, newexpr_bool(false), NULL,$$, -1 , yylineno);
		                                        emit(jump,NULL,NULL,NULL,nextquad()+2, yylineno);}
    | expr AND expr                           { DEBUG_REDUCE("expr -> expr and expr"); $$ = emit_relop_op(and_op, $1, $3); }
    | expr OR expr                            { DEBUG_REDUCE("expr -> expr or expr");  $$ = emit_relop_op(or_op, $1, $3); }
    | term                                    { DEBUG_REDUCE("expr -> term"); $$ = $1; }
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
       {          assign_error($1); DEBUG_REDUCE("assignexpr -> lvalue = expr");             

                    // Handle table item assignments
                    if ($1->type == tableitem_e) {
                        emit(tablesetelem, $1, $1->index, $3, -1, yylineno);
                        $$ = emit_iftableitem($1);
                        $$->type = assignexpr_e;
                    } else {
                        
                        $$ = emit_assign_expr($1, $3);
                        // Create a new temporary expression for the result
                        $assignexpr = newexpr(var_e); 
						$assignexpr->sym = newtemp();
						emit(assign, $assignexpr, $lvalue, NULL, 0, yylineno);
                    }
                }
        
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
      IDENT                                 {SymbolTableEntry_T entry = add_ident(*$1); $$ = symEntr_to_expr(entry); DEBUG_REDUCE("lvalue -> IDENT"); }
    | LOCAL IDENT                           {SymbolTableEntry_T entry = add_local_dent(*$2); $$ = symEntr_to_expr(entry);  DEBUG_REDUCE("lvalue -> local IDENT"); }
    | NAMESPACE IDENT                       {SymbolTableEntry_T entry = handle_namespace_dent(*$2); $$ = symEntr_to_expr(entry); DEBUG_REDUCE("lvalue -> ::IDENT"); }
    | member                                {$$ = NULL;  DEBUG_REDUCE("lvalue -> member"); }
    ;

member:
      lvalue DOT IDENT                          {$$ = lvalue_id_handler($1, *$3); DEBUG_REDUCE("member -> lvalue . IDENT"); }
    | lvalue LEFT_BRACKET expr RIGHT_BRACKET    {$$ = member_handler($1, $3); DEBUG_REDUCE("member -> lvalue [expr]");     }
    | call DOT IDENT                            {$$ = member_item($1, *$3); DEBUG_REDUCE("member -> call . IDENT");         }
    | call LEFT_BRACKET expr RIGHT_BRACKET      {$$ = member_handler($1, $3); DEBUG_REDUCE("member -> call [expr]");       }
    ;

call:
        lvalue callsuffix {DEBUG_REDUCE("call -> lvalue callsuffix"); }
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
      DOUBLE_DOT IDENT LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
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
      FUNCTION {enter_func(0, "");}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS {scope--;add_anon_function(args);}
            block {exit_func(0, ""); DEBUG_REDUCE("funcdef -> function(idlist) block"); }
                                          
    | FUNCTION IDENT {enter_func(1, *$2);}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS 
            {scope--; add_function(*$2, args);}
            block
            {exit_func(1, *$2); DEBUG_REDUCE("funcdef -> function IDENT(idlist) block"); }
    ;


const:
      INTEGER     { $$ = newexpr(constnum_e); $$->numConst = $1;  DEBUG_REDUCE("const -> INTEGER"); }
    | REALCONST   { $$ = newexpr(constnum_e); $$->numConst = $1;  DEBUG_REDUCE("const -> REALCONST"); }
    | STRING      {  DEBUG_REDUCE("const -> STRING"); }
    | NIL         { $$ = newexpr(nil_e); DEBUG_REDUCE("const -> NIL"); }
    | TRUE        { $$ = newexpr(constbool_e); $$->boolConst = true; DEBUG_REDUCE("const -> TRUE"); }
    | FALSE       { $$ = newexpr(constbool_e); $$->boolConst = false; DEBUG_REDUCE("const -> FALSE"); }
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

ifprefix 
: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {
    
    emit(if_eq, newexpr_bool('1'), $3, newexpr_constnum(nextquad() + 2), currQuad, yylineno);

    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();

    emit(jump, NULL, NULL, 0, currQuad, yylineno); 

   
}
;

elseprefix 
: ELSE {

    $$ = newexpr(constnum_e);

    $$->numConst = nextquad();

    emit(jump, NULL, NULL, 0, currQuad, yylineno); 

}
;

ifstmt 
: ifprefix stmt {

      patchlabel((int)$1->numConst, nextquad());
     
  }
| ifprefix stmt elseprefix stmt {

      patchlabel((int)$1->numConst, (int)$3->numConst + 1);

      patchlabel((int)$3->numConst, nextquad());
     
  }
;

whilestart : WHILE {
   
    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();
   
}
;

whilecond : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {
   
    emit(if_eq, $2, newexpr_bool('1'), newexpr_constnum(nextquad() + 2), nextquadlabel(), yylineno);
    

    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();
   
    emit(jump, NULL, NULL, 0,0,0);
    
}
;

whilestmt:
    whilestart whilecond stmt {

        make_stmt($3);
       
        emit(jump, NULL, NULL, $1,nextquadlabel(), yylineno);
    
        patchlabel((int)$2->numConst, nextquad());
       
        patchlist($3->breakList, nextquad());
        
        patchlist($3->contList, nextquad());
        
    }
;

M : {
    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();
    emit(jump,NULL,NULL,0,-1,yylineno);
}
;

N : {
    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();
}
;

forprefix : FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON {

    make_loop_t($$);

    $$->test = $5->numConst;
    $$->enter = nextquad();

    emit(if_eq,$6,newexpr_bool('1'),0,-1,yylineno);

}
;

forstmt: forprefix N elist RIGHT_PARENTHESIS N stmt N {

    patchlabel($1->enter,(int)$5->numConst+1);
    patchlabel((int)$2->numConst,nextquad());
    patchlabel((int)$5->numConst,$1->test);
    patchlabel((int)$7->numConst,(int)$2->numConst+1);

    make_stmt($6);

    patchlist($6->breakList,nextquad());
    patchlist($6->contList,(int)$2->numConst + 1);

}
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
   // printFullSymTable(oSymTable); 
    print_quads();
    return 0;
}

void yyerror(const char *s) {
    std::cerr << "Parse error: " << s << " at line : "<<yylineno <<" before token: " <<  yytext << endl;
}
