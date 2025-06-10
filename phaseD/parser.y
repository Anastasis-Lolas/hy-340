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
extern int              infunction;
void yyerror            (const char* yaccProvidedMessage);
extern int              yylineno;
extern int              yylex(void);
extern char*            yytext;
extern int              yylex(void);
extern unsigned int     scope;
extern unsigned int     currQuad;
std::vector<void *>     args;
extern std::vector<quad*> quad_table;

//#define DEBUG_REDUCE(msg) std::cout << "Reduced: " << msg << " (line " << yylineno << ")\n"
#define DEBUG_REDUCE(msg)

%}
%code requires {
    #include "Symtable/TableEntry/SymbolTableEntry.h"
    #include "Quads/expression.h"
    #include "Quads/quad.h"
    #include "expr_handler.h"
    #include "t-codeLib/t-code.h"
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
    call_t* callVal;
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

%type <s> stmt stmt_list returnstmt block forstmt whilestmt ifstmt loopstmt Continue Break
%type <exprVal> member assignexpr term primary indexed indexedelem objectdef
%type <exprVal> expr call const elist elist_tail
%type <exprVal> lvalue
%type <stringValue> IDENT
%type <idList> idlist
%type <exprVal> ifprefix elseprefix  M N
%type <loop_t> forprefix
%type <symEntry> funcdef
%type<callVal> callsuffix normcall methodcall
%type <intValue> whilestart whilecond
%type<s> program

%right ASSIGN

%left OR 

%left AND


%nonassoc NOT_EQUALS EQUAL

%nonassoc GREATER GREATER_EQUAL LESS LESS_EQUAL




%left PLUS MINUS
%left MULT DIV MOD
%left UMINUS
%right NOT 

%right PLUS_PLUS MINUS_MINUS 


%left DOT DOUBLE_DOT


%left LEFT_PARENTHESIS RIGHT_PARENTHESIS
%left LEFT_BRACE RIGHT_BRACE 




%%

program:
      stmt_list     {$$=$1; DEBUG_REDUCE("program -> stmt_list"); }
   
    ;

stmt_list : stmt_list stmt {
                             $$ = stmt_list_handler($1,$2);
                             DEBUG_REDUCE("stmt_list -> stmt ;"); 
                           }
            | stmt {$$ = $1;DEBUG_REDUCE("stmt list  -> stmt ;"); }
          ;

stmt:
      expr SEMICOLON      {  
                            $$ = new stmt_t(); make_stmt($$);
                            if ($1->type == boolexpr_e)
                                boolify_expr($1);
                            resettemp();
      
                            DEBUG_REDUCE("stmt -> expr ;"); 
                          }
    | ifstmt              {$$ = $1; DEBUG_REDUCE("stmt -> ifstmt");     }
    | whilestmt           {$$ = $1;  DEBUG_REDUCE("stmt -> whilestmt");  }
    | forstmt             {$$ = $1; DEBUG_REDUCE("stmt -> forstmt");    }
    | returnstmt          {$$ = $1; DEBUG_REDUCE("stmt -> returnstmt"); }
    | Break               {$$ = $1; DEBUG_REDUCE("stmt -> break ;");    }
    | Continue            {$$ = $1; DEBUG_REDUCE("stmt -> continue ;"); }
    | block               {$$ = $1; DEBUG_REDUCE("stmt -> block");      }
    | funcdef             {
                            $$ = new stmt_t();
                            make_stmt($$); 
                            DEBUG_REDUCE("stmt -> funcdef"); 
                          }
    | SEMICOLON           {$$ = new stmt_t();make_stmt($$); DEBUG_REDUCE("stmt -> ;"); }
    ;




expr:
    assignexpr                              { $$ = $1; DEBUG_REDUCE("expr -> assignexpr"); }
    
    | expr PLUS expr
                                            {
                                            if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                std::cout << "Illegal use of '+' with non-numeric expression at line " << yylineno << std::endl;                                        
                                            } else {
                                                DEBUG_REDUCE("expr -> expr + expr");
                                                $$ = emit_arith_op(add, $1, $3);
                                                }
                                            }
    | expr MINUS expr
                                            {
                                            if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                $$ = newexpr(nil_e);
                                                std::cout << "Illegal use of '-' with non-numeric expression at line " << yylineno << std::endl;   
                                                } else {
                                                    DEBUG_REDUCE("expr -> expr - expr");
                                                    $$ = emit_arith_op(sub, $1, $3);
                                                }
                                            }
    | expr MULT expr
                                            {
                                            if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                std::cout << "Illegal use of '*' with non-numeric expression at line " << yylineno << std::endl;
                                            } else {
                                                DEBUG_REDUCE("expr -> expr * expr");
                                                $$ = emit_arith_op(mul, $1, $3);
        }
      }

    | expr DIV expr
                                            {
                                            if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {                                                   
                                                std::cout << "Illegal use of '/' with non-numeric expression at line " << yylineno << std::endl;
                                            } else {
                                                DEBUG_REDUCE("expr -> expr / expr");
                                                $$ = emit_arith_op(divv, $1, $3);
                                                }
                                            }

    | expr MOD expr
                                            {
                                            if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                std::cout << "Illegal use of '%' with non-numeric expression at line " << yylineno << std::endl;
                                            } else {
                                                DEBUG_REDUCE("expr -> expr % expr");
                                                $$ = emit_arith_op(mod, $1, $3);
                                                }
                                            }
    | expr GREATER expr {
                                                DEBUG_REDUCE("expr -> expr > expr");
       
                                                if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                    std::cout << "Illegal use of '>' with non-numeric expression at line " << yylineno << std::endl;
                                                   
                                                }
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->sym = newtemp();
                                                    $$->truelist.push_back(nextquad());
                                                    $$->falselist.push_back(nextquad() + 1);
                                                    emit(if_greater, $1, $3, NULL, 0, yylineno);
                                                    emit(jump, NULL, NULL, NULL, 0, yylineno);
      }
	| expr LESS expr {
                                                DEBUG_REDUCE("expr -> expr < expr");
       
                                                if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                    std::cout << "Illegal use of '<' with non-numeric expression at line " << yylineno << std::endl;
                                                   
                                                }
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->sym = newtemp();
                                                    $$->truelist.push_back(nextquad());
                                                    $$->falselist.push_back(nextquad() + 1);
                                                    emit(if_less, $1, $3, NULL, 0, yylineno);
                                                    emit(jump, NULL, NULL, NULL, 0, yylineno);
      }                                                                                                  
    | expr GREATER_EQUAL expr {
                                                DEBUG_REDUCE("expr -> expr >= expr");

                                                if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                    std::cout << "Illegal use of '>=' with non-numeric expression at line " << yylineno << std::endl;
                                                    
                                                } 
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->sym = newtemp();
                                                    $$->truelist.push_back(nextquad());
                                                    $$->falselist.push_back(nextquad() + 1);
                                                    emit(if_greatereq, $1, $3, NULL, 0, yylineno);
                                                    emit(jump, NULL, NULL, NULL, 0, yylineno);
                                                                                    }                                
    | expr LESS_EQUAL expr                      { DEBUG_REDUCE("expr -> expr <= expr");
                                                if (!check_arithmetic_expr($1) || !check_arithmetic_expr($3)) {
                                                    std::cout << " Illegal use of '<' with non-numeric expression at line " << yylineno << std::endl;
                                
                                                }  
                                                $$ = newexpr(boolexpr_e);
                                                $$->sym = newtemp();
                                                $$->truelist.push_back(nextquad());
                                                $$->falselist.push_back(nextquad() + 1);
                                                emit(if_lesseq, $1, $3, NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }

    | expr EQUAL expr                         { DEBUG_REDUCE("expr -> expr == expr");
                                                boolify_expr($1);
                                                boolify_expr($3);
                                                $$ = newexpr(boolexpr_e);
                                                //$$->sym = newtemp();
                                                $$->truelist.push_back(nextquad());
                                                $$->falselist.push_back(nextquad() + 1);
                                                emit(if_eq, $1, $3, NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }

    | expr NOT_EQUALS expr                    { DEBUG_REDUCE("expr -> expr != expr");  
                                                boolify_expr($1);
                                                boolify_expr($3);
                                                $$ = newexpr(boolexpr_e);
                                                //$$->sym = newtemp();
                                                $$->truelist.push_back(nextquad());
                                                $$->falselist.push_back(nextquad() + 1);
                                                emit(if_noteq, $1, $3, NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }

    | expr AND {$1 = to_boolexpr($1) ; } M expr                           { 
                                                    DEBUG_REDUCE("expr -> expr and expr");
                                                    $5 = to_boolexpr($5);  
                                                    backpatch($1->truelist, (unsigned)($4->numConst)); 
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->truelist = $5->truelist;
                                                    $$->falselist = merge($1->falselist, $5->falselist);
}

    | expr OR{$1 = to_boolexpr($1) ; }   M expr                            {
                                                   DEBUG_REDUCE("expr -> expr or expr");
                                                   
                                                    $5 = to_boolexpr($5);
                                                    backpatch($1->falselist, (unsigned)$4->numConst);
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->truelist = merge($1->truelist, $5->truelist);
                                                    $$->falselist = $5->falselist;
} 
    | term                                    {  $$ = $1; DEBUG_REDUCE("expr -> term");  }
    ;


term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
                                                    { $$ = $2; 
                                                    // Pass the expr unchanged
                                                    DEBUG_REDUCE("term -> (expr)"); }
    | MINUS expr%prec UMINUS {
                                                    DEBUG_REDUCE("term -> -expr");
                                                    check_arith($2, "unary minus");
                                                    $$ = newexpr(arithexpr_e);
                                                    $$->sym = newtemp();
                                                   emit(uminus, NULL, $2, $$, -1, yylineno);}
    | NOT expr
                                                    { 
                                                    DEBUG_REDUCE("term -> not expr");
                                                    expr* b = to_boolexpr($2);
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->truelist = b->falselist;   
                                                    $$->falselist = b->truelist;
    }
    | PLUS_PLUS lvalue
                                                    {temrs_error($2,"++lvalue");
                                                    check_arith($2,"++lvalue");
                                                    if($2->type == tableitem_e) {
									                    $$ = emit_iftableitem($2);
									                    emit(add, $$, newexpr_constdouble(1), $$, -1 , yylineno);
									                    emit(tablesetelem,$2->index, $$,  $2, -1, yylineno);
												    }
									                else {
													    emit(add, $2, newexpr_constdouble(1), $2,-1, yylineno);
													    $$ = newexpr(arithexpr_e);
													    $$->sym = newtemp();
													    emit(assign, $2, NULL, $$,-1, yylineno);
												    }
                                                    DEBUG_REDUCE("term -> ++lvalue"); }
    | lvalue PLUS_PLUS
                                                    {temrs_error($1,"++"); 
                                                    check_arith($1,"lvalue++");
                                                    $$ = newexpr(var_e);
                                                    $$->sym = newtemp();
                                                    if ($1->type == tableitem_e) {
                                                        expr* val = emit_iftableitem($1);

                                                        emit(assign, val, NULL, $$,-1, yylineno);
                                                        emit(add, val, newexpr_constdouble(1), val,-1, yylineno);
                                                        emit(tablesetelem,$1->index, val,  $1,-1, yylineno);
                                                    } else {
                                                        emit(assign, $1, NULL, $$, -1, yylineno); // Copy current value
                                                        emit(add, $1, newexpr_constdouble(1), $1, -1, yylineno); // Update lvalue
                                                    }
                                                    DEBUG_REDUCE("term -> lvalue++"); }


    | MINUS_MINUS lvalue
                                                    {temrs_error($2,"--"); 
                                                    check_arith($2, "--lvalue");
                                                    if ($2->type == tableitem_e) {
                                                        $$ = emit_iftableitem($2);
                                                        emit(sub, $$, newexpr_constdouble(1), $$, -1, yylineno);
                                                        emit(tablesetelem, $2->index, $$,  $2, -1, yylineno);
                                                    } else {
                                                        emit(sub, $2, newexpr_constdouble(1), $2, -1, yylineno);
                                                        $$ = newexpr(arithexpr_e);
													    $$->sym = newtemp();
                                                        emit(assign, $2, NULL, $$, -1, yylineno);
                                                    }
                                                    DEBUG_REDUCE("term -> --lvalue"); }
    | lvalue MINUS_MINUS
                                                    {temrs_error($1,"--"); 
                                                    check_arith($1, "lvalue --");
                                                    $$ = newexpr(var_e);
                                                    $$->sym = newtemp();
                                                    if ($1->type == tableitem_e) {
                                                        
                                                        expr* val = emit_iftableitem($1);
                                                        emit(assign, val, NULL, $$, -1, yylineno);
                                                        emit(sub, val , newexpr_constdouble(1), val, -1, yylineno);
                                                        emit(tablesetelem, $1->index, val,  $1, -1, yylineno);
                                                    } else {
                                                        emit(assign, $1, NULL, $$, -1, yylineno);
                                                        emit(sub, $1, newexpr_constdouble(1), $1, -1, yylineno);
                                                    }
                                                    DEBUG_REDUCE("term -> lvalue--"); }
    | primary
        {$$ = $1; DEBUG_REDUCE("term -> primary"); }
    ;


assignexpr:
    lvalue ASSIGN expr
    {
        assign_error($1);
        DEBUG_REDUCE("assignexpr -> lvalue = expr");

        expr* rval = $3;

       if (rval->type == boolexpr_e) {
            rval = boolify_expr(rval);  // temp with true/false
        }

        if ($1->type == tableitem_e) {
            emit(tablesetelem,$1->index, rval, $1,   -1, yylineno);
            $$ = emit_iftableitem($1);
            $$->type = var_e;
        } else {
            // assign rval in lvalue
            emit(assign, rval, NULL, $1, -1, yylineno);

         
            $$ = newexpr(var_e);
            $$->sym = newtemp();
            emit(assign, $1, NULL, $$, -1, yylineno); 
            
        }
    };

primary:
      lvalue                                {$$ = emit_iftableitem($1); DEBUG_REDUCE("primary -> lvalue");}
    | call                                  { DEBUG_REDUCE("primary -> call");      }
    | objectdef                             { DEBUG_REDUCE("primary -> objectdef"); }
    | LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS
                                            {$$ = newexpr(programfunc_e); $$->sym = $2; DEBUG_REDUCE("primary -> (funcdef)"); }                                 
    | const                                 { DEBUG_REDUCE("primary -> const");  }
    ;

lvalue:
      IDENT                                 {SymbolTableEntry_T entry = add_ident(*$1); $$ = symEntr_to_expr(entry); DEBUG_REDUCE("lvalue -> IDENT"); }
    | LOCAL IDENT                           {SymbolTableEntry_T entry = add_local_dent(*$2); $$ = symEntr_to_expr(entry);  DEBUG_REDUCE("lvalue -> local IDENT"); }
    | NAMESPACE IDENT                       {SymbolTableEntry_T entry = handle_namespace_dent(*$2); $$ = symEntr_to_expr(entry); DEBUG_REDUCE("lvalue -> ::IDENT"); }
    | member                                {$$ = $1;  DEBUG_REDUCE("lvalue -> member"); }
    ;

member:
      lvalue DOT IDENT                          {$$ = lvalue_id_handler($1, *$3); DEBUG_REDUCE("member -> lvalue . IDENT"); }
    | lvalue LEFT_BRACKET expr RIGHT_BRACKET    {$$ = member_handler($1, $3); DEBUG_REDUCE("member -> lvalue [expr]");     }
    | call DOT IDENT                            {$$ = member_item($1, *$3); DEBUG_REDUCE("member -> call . IDENT");         }
    | call LEFT_BRACKET expr RIGHT_BRACKET      {$$ = member_handler($1, $3); DEBUG_REDUCE("member -> call [expr]");       }
    ;

call:
        lvalue callsuffix   {
                                $1 = emit_iftableitem($1);
                                if($2->method){
                                    expr* self = $1;
                                    self = member_item(self, $2->name);
                                    $1 = emit_iftableitem(self);
                                    self->next = $2->elist;
                                    $2->elist = self;
                                }
                                $$ = call_handler($1, $2->elist);
                                DEBUG_REDUCE("call -> lvalue callsuffix"); 
                            }
    | call LEFT_PARENTHESIS elist RIGHT_PARENTHESIS {$$ = call_handler($1, $3); DEBUG_REDUCE("call -> call(elist)"); }
    | LEFT_PARENTHESIS funcdef RIGHT_PARENTHESIS LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
                                           {$$ = anonym_call($2, $5); DEBUG_REDUCE("call -> (funcdef)(elist)"); }
    ;

callsuffix:
      normcall                              {$$ = $1; DEBUG_REDUCE("callsuffix -> normcall"); }
    | methodcall                            {$$ = $1; DEBUG_REDUCE("callsuffix -> methodcall"); }
    ;

normcall:
      LEFT_PARENTHESIS elist RIGHT_PARENTHESIS{$$ = normcall_handler($2);DEBUG_REDUCE("normcall -> (elist)");}
    ;

methodcall:
      DOUBLE_DOT IDENT LEFT_PARENTHESIS elist RIGHT_PARENTHESIS
                                           {$$ = methodcall_handler($4, *$2); DEBUG_REDUCE("methodcall -> ::IDENT(elist)"); }
    ;

elist:
      /* empty */                           { $$ = nullptr;           }
    | expr elist_tail                       { 
                                                if($1->type == boolexpr_e) {
                                                    $1 = boolify_expr($1);
                                                } 
                                                $1->next = $2; $$ = $1; 
                                            }
    ;

elist_tail:
      COMMA expr elist_tail                 { 
                                                if($2->type == boolexpr_e) {
                                                    $2 = boolify_expr($2);
                                                } 
                                                $2->next = $3; $$ = $2;   
                                            }
    | /* empty */                           { $$ = nullptr;             }
    ;

objectdef:
      LEFT_BRACKET elist RIGHT_BRACKET          {
                                                    expr* t = newexpr(newtable_e);
                                                    t->sym = newtemp();
                                                    emit(tablecreate,  NULL, NULL, t,-1, yylineno);
                                                    for (int i = 0; $elist; $elist = $elist->next)
                                                        emit(tablesetelem, newexpr_constdouble(i++), $elist,  t, -1, yylineno);
                                                    $$ = t;
                                                    DEBUG_REDUCE("objectdef -> {elist}"); 
                                                }

      | LEFT_BRACKET indexed RIGHT_BRACKET      { 
                                                    expr* t = newexpr(newtable_e);
                                                    t->sym = newtemp();
                                                    emit(tablecreate,  NULL, NULL, t, -1, yylineno);
                                                    while($2){
                                                        emit(tablesetelem, $2->index, $2, t, -1, yylineno);
                                                        $2 = $2->next;
                                                    }
                                                    $$ = t;
                                                    DEBUG_REDUCE("objectdef -> indexed ");  
                                                }
    ;

indexed:
      indexedelem                          {$$ = $1;  DEBUG_REDUCE("indexed -> indexedelem"); }
    | indexed COMMA indexedelem            {
                                                    while($1->next){  
                                                        $1 = $1->next;
                                                    }        
                                                    $1->next = $3;   
                                                    DEBUG_REDUCE("indexed -> indexed , indexedelem"); 
                                            }
    ;

indexedelem:
      LEFT_BRACE expr COLON expr RIGHT_BRACE
                                           {$4->index = $2; $$ = $4; DEBUG_REDUCE("indexedelem -> [expr : expr]"); }
    ;

block:
      LEFT_BRACE { scope++; } stmt_list RIGHT_BRACE {$$=$3; exit_block(); DEBUG_REDUCE("block -> {stmt_list}"); }
      |  LEFT_BRACE{scope++;} RIGHT_BRACE {exit_block(); $$=new stmt_t();make_stmt($$); DEBUG_REDUCE("block -> {}");   }
    
    ;
    

funcdef:
      FUNCTION M {enter_func(0, "");}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS {scope--;add_anon_function(args, $2->numConst + 1);}
            block {
                    $$ = exit_func(0, "", $8->returnList); 
                    quad_table[$$->value.funcVal->funcIndex]->result->sym = $$;
                    //assert(quad_table[$$->value.funcVal->funcIndex]->result);
                    DEBUG_REDUCE("funcdef -> function(idlist) block"); 
                  }
                                          
    | FUNCTION IDENT M {enter_func(1, *$2);}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS 
            {scope--; add_function(*$2, args, $3->numConst + 1);}
            block
            {
                $$ = exit_func(1, *$2, $9->returnList); 
                quad_table[$$->value.funcVal->funcIndex]->result->sym = $$;
                DEBUG_REDUCE("funcdef -> function IDENT(idlist) block"); 

            }
    ;


const:
      INTEGER     { $$ = newexpr_intnum($1);   DEBUG_REDUCE("const -> INTEGER"); }
    | REALCONST   { $$ = newexpr(constdouble_e); $$->numConst = $1;  DEBUG_REDUCE("const -> REALCONST"); }
    | STRING      {$$ = newexpr_conststring(*$1); delete $1; DEBUG_REDUCE("const -> STRING"); }
    | NIL         { $$ = newexpr_nill(); DEBUG_REDUCE("const -> NIL"); }
    | TRUE        { $$ = newexpr_bool(true); DEBUG_REDUCE("const -> TRUE"); }
    | FALSE       { $$ = newexpr_bool(false); DEBUG_REDUCE("const -> FALSE"); }
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
: IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS 
{
    if($3->type == boolexpr_e) {
       $3 = boolify_expr($3);  // Evaluate condition into a temporary
    }
    emit(if_eq, $3, newexpr_bool(true), NULL, nextquad() + 2, yylineno);  // Jump to body if true
    $$ = newexpr(constdouble_e);
    $$->numConst = nextquad();
    emit(jump, NULL, NULL, NULL, 0, yylineno);  // Jump to skip body if false
}
;

elseprefix 
: ELSE {

    $$ = newexpr(constdouble_e);
    $$->numConst = nextquad();        
    emit(jump, NULL, NULL, NULL, 0, yylineno);

}
;

ifstmt 
: ifprefix stmt {
        $$ = $2;
       patchlabel((int)$1->numConst, nextquad());
     
  }
| ifprefix stmt elseprefix stmt {

      patchlabel((int)$1->numConst, (int)$3->numConst + 1);
      patchlabel((int)$3->numConst, nextquad());

      $$ = stmt_list_handler($2, $4);
     
  }
;

loopstart :                        {++loopcounter;}
          ;
loopend   :                        {--loopcounter;}
          ;
loopstmt  : loopstart stmt loopend {$$ = $2;}
          ;

whilestart : WHILE {
   
    $$ = nextquad();
}
;

whilecond : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS 
{
    if ($2->type == boolexpr_e) {
        $2 = boolify_expr($2);
    }
    emit(if_eq, $2, newexpr_bool(1), NULL, nextquad() + 2, yylineno); // True -> body
    $$ = nextquad(); // False jump
    emit(jump, NULL, NULL, NULL, 0, yylineno); // False -> exit
};

whilestmt:
    whilestart whilecond loopstmt {
        unsigned int loopStart = $1;
        unsigned int falseJump = $2;
        stmt_t* body = $3;

        if (!body) {
            body = new stmt_t();
            make_stmt(body);
        }

        emit(jump, nullptr, nullptr, nullptr, loopStart, yylineno);

        patchlabel(falseJump, nextquad());

        patchlist(body->breakList, nextquad());

        patchlist(body->contList, loopStart);

        body->breakList = 0;
        body->contList = 0;


        $$ = body;
    };

N : {

    $$ = newexpr(constdouble_e);
    $$->numConst = nextquad();
    emit(jump, NULL, NULL, NULL, 0, yylineno); 

}
;

M : {

    $$ = newexpr(constdouble_e);
    $$->numConst = nextquad();

}
;

forprefix : FOR LEFT_PARENTHESIS elist SEMICOLON M expr SEMICOLON {

    $$ = new forloop_t();
    make_loop_t($$);
    $$->test = $5->numConst;
    if ($6->type == boolexpr_e) {
        $6 = boolify_expr($6);
    }
    $$->enter = nextquad();
    emit(if_eq,$6,newexpr_bool(1),0,-1,yylineno);
    
}
;

forstmt: forprefix N elist RIGHT_PARENTHESIS N loopstmt N {
     
    patchlabel($1->enter,(int)$5->numConst+1); //true jump 
     
    patchlabel((int)$2->numConst,nextquad()); //false jump 

    patchlabel((int)$5->numConst,$1->test); // loop jump
 
    patchlabel((int)$7->numConst,(int)$2->numConst+1); //closure jump

    patchlist($6->breakList,nextquad());
    patchlist($6->contList,(int)$2->numConst + 1);

    $6->breakList = 0;
    $6->contList = 0;
    //make_stmt($6);
    $$ = $6;
    }
;


Break : BREAK SEMICOLON {

    if(loopcounter == 0) {
        yyerror("break statement is outside of the loop ");
        $$ = new stmt_t();  
        make_stmt($$);
    } else {
        $$ = new stmt_t();  
        make_stmt($$);
        $$->breakList = nextquad();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
    }
}



Continue : CONTINUE SEMICOLON { 
  if(loopcounter == 0) {
        yyerror("continue statement is outside of the loop ");
        $$ = new stmt_t();
        make_stmt($$);
    } else {
        $$ = new stmt_t();  
        make_stmt($$);
        $$->contList = nextquad();
        emit(jump, NULL, NULL, NULL, 0, yylineno);
    }
}

returnstmt:
      RETURN SEMICOLON
        {
            if(infunction==0){
                std::cerr <<"Error at line " << yylineno << ": using return statement outside of function\n";
            }  
            emit(ret, nullptr, nullptr, nullptr, -1, nextquadlabel());
            $$ = new stmt_t();
            make_stmt($$);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
            $$->returnList = nextquadlabel() - 1;
            DEBUG_REDUCE("returnstmt -> return ;");
        }
    | RETURN expr SEMICOLON
        {
            if(infunction==0){
                std::cerr <<"Error at line " << yylineno << ": using return statement outside of function\n";
            }  
            emit(ret,  nullptr, nullptr, $2, -1, nextquadlabel());
            $$ = new stmt_t();
            make_stmt($$);
            emit(jump, NULL, NULL, NULL, 0, yylineno);
            $$->returnList = nextquadlabel() - 1;
            DEBUG_REDUCE("returnstmt -> return expr ;"); 
        }
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
    push_loopcounter();
    yyparse();

    //print_args(args);
   // printFullSymTable(oSymTable); 
    print_quads();
        
    generate_instructions();
    print_instructions();
    std::cout<<"print_const_strings:\n";
    print_const_strings();
    std::cout<<"print_const_nums:\n";
    print_const_nums();
    std::cout<<"print_userfuncs:\n";
    print_userfuncs();
    std::cout<<"print_libfuncs:\n";
    print_libfuncs();


   generate_txt_read("peos.txt");
   generate_binary_readable("bifile.abc");



    //void free_instructions();



    return 0;
}

void yyerror(const char *s) {
    std::cerr << "Parse error: " << s << " at line : "<<yylineno <<" before token: " <<  yytext << endl;
}
