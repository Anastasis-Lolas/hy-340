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


#define DEBUG_REDUCE(msg) std::cout << "Reduced: " << msg << " (line " << yylineno << ")\n"
//#define DEBUG_REDUCE(msg)

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
%type <exprVal> member assignexpr term primary 
%type <exprVal> expr call const elist elist_tail
%type <exprVal> lvalue
%type <stringValue> IDENT
%type <idList> idlist
%type <exprVal> ifprefix elseprefix whilestart whilecond M N
%type <loop_t> forprefix
%type <symEntry> funcdef
%type<callVal> callsuffix normcall methodcall




%right ASSIGN
%left OR 
%left AND
%nonassoc GREATER GREATER_EQUAL LESS LESS_EQUAL
%nonassoc NOT_EQUALS EQUAL
%right NOT 
%left MULT DIV  MOD 
%right PLUS MINUS
%left UMINUS
%left LEFT_PARENTHESIS RIGHT_PARENTHESIS
%left LEFT_BRACE RIGHT_BRACE 
%left DOT DOUBLE_DOT
%right PLUS_PLUS MINUS_MINUS 




%%

program:
      stmt_list     { DEBUG_REDUCE("program -> stmt_list"); }
   
    ;

stmt_list : stmt_list stmt {
                             $$ = stmt_list_handler($1,$2);
                             DEBUG_REDUCE("stmt -> expr ;"); 
                           }
            | {$$ = new stmt_t(); make_stmt($$);DEBUG_REDUCE("stmt list  -> empty ;"); }
          ;

stmt:
      expr SEMICOLON      {  
                            $$ = new stmt_t();
                            make_stmt($$);
                            DEBUG_REDUCE("stmt -> expr ;"); 
                          }
    | ifstmt              {$$ = $1; DEBUG_REDUCE("stmt -> ifstmt"); }
    | whilestmt           {$$ = $1; DEBUG_REDUCE("stmt -> whilestmt"); }
    | forstmt             {$$ = $1; DEBUG_REDUCE("stmt -> forstmt"); }
    | returnstmt          {$$ = $1; DEBUG_REDUCE("stmt -> returnstmt"); }
    | Break               { DEBUG_REDUCE("stmt -> break ;"); }
    | Continue            { DEBUG_REDUCE("stmt -> continue ;"); }
    | block               {$$ = $1; DEBUG_REDUCE("stmt -> block"); }
    | funcdef             {
                            $$ = new stmt_t();
                            make_stmt($$); 
                            DEBUG_REDUCE("stmt -> funcdef"); 
                          }
    | SEMICOLON           {DEBUG_REDUCE("stmt -> ;"); }
    ;




expr:
      assignexpr                              { $$ = $1; DEBUG_REDUCE("expr -> assignexpr"); }
    | expr PLUS expr                          { DEBUG_REDUCE("expr -> expr + expr");  $$ = emit_arith_op(add, $1, $3); }
    | expr MINUS expr                         { DEBUG_REDUCE("expr -> expr - expr");   $$ = emit_arith_op(sub, $1, $3); }
    | expr MULT expr                          { DEBUG_REDUCE("expr -> expr * expr");   $$ = emit_arith_op(mul, $1, $3); }
    | expr DIV expr                           { DEBUG_REDUCE("expr -> expr / expr");   $$ = emit_arith_op(divv, $1, $3); }
    | expr MOD expr                           { DEBUG_REDUCE("expr -> expr % expr");   $$ = emit_arith_op(mod, $1, $3); }
    | expr GREATER expr                       { DEBUG_REDUCE("expr -> expr > expr");   
                                                
                                                    expr* left = boolify_expr($1);
                                                    expr* right = boolify_expr($3);

                                                    $$ = newexpr(boolexpr_e);
                                                    $$->sym = newtemp();

                                                    $$->truelist.push_back(nextquad());
                                                    $$->falselist.push_back(nextquad() + 1);

                                                    emit(if_greater, left, right, NULL, 0, yylineno);
                                                    emit(jump, NULL, NULL, NULL, 0, yylineno);
}
		                                                                                                    
    | expr GREATER_EQUAL expr                 { DEBUG_REDUCE("expr -> expr >= expr");   
                                                $$ = newexpr(boolexpr_e);
                                                $$->sym = newtemp();
                                                $$->truelist.push_back(nextquad());
                                                $$->falselist.push_back(nextquad() + 1);
                                                emit(if_greatereq, $1, $3, NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }

    | expr LESS expr                          { DEBUG_REDUCE("expr -> expr < expr");   
		                                            expr* left = boolify_expr($1);
                                                    expr* right = boolify_expr($3);

                                                    $$ = newexpr(boolexpr_e);
                                                    $$->sym = newtemp();

                                                    $$->truelist.push_back(nextquad());
                                                    $$->falselist.push_back(nextquad() + 1);

                                                    emit(if_less, left, right, NULL, 0, yylineno);
                                                    emit(jump, NULL, NULL, NULL, 0, yylineno);
                                                    
                                                }
    | expr LESS_EQUAL expr                    { DEBUG_REDUCE("expr -> expr <= expr");  
                                                $$ = newexpr(boolexpr_e);
                                                $$->sym = newtemp();
                                                $$->truelist.push_back(nextquad());
                                                $$->falselist.push_back(nextquad() + 1);
                                                emit(if_lesseq, $1, $3, NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }

    | expr EQUAL expr                         { DEBUG_REDUCE("expr -> expr == expr"); 
                                                $$ = newexpr(boolexpr_e);
                                                $$->sym = newtemp();
                                                $$->truelist.push_back(nextquad());
                                                $$->falselist.push_back(nextquad() + 1);
                                                emit(if_eq, $1, $3, NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }

    | expr NOT_EQUALS expr                    { DEBUG_REDUCE("expr -> expr != expr");  
                                                $$ = newexpr(boolexpr_e);
                                                $$->sym = newtemp();
                                                $$->truelist.push_back(nextquad());
                                                $$->falselist.push_back(nextquad() + 1);
                                                emit(if_noteq, $1, $3, NULL, 0, yylineno);
                                                emit(jump, NULL, NULL, NULL, 0, yylineno); }

    | expr AND M expr                           { DEBUG_REDUCE("expr -> expr and expr");
                                                    $1 = to_boolexpr($1);  // Ensure expr1 is boolexpr_e
                                                    $4 = to_boolexpr($4);  // Ensure expr2 is boolexpr_e
                                                    backpatch($1->truelist, (unsigned)$3->numConst);
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->truelist = $4->truelist;
                                                    $$->falselist = merge($1->falselist, $4->falselist);
}

    | expr OR M expr                            {
                                                   DEBUG_REDUCE("expr -> expr or expr");
                                                    $1 = to_boolexpr($1);
                                                    $4 = to_boolexpr($4);
                                                    backpatch($1->falselist, (unsigned)$3->numConst);
                                                    $$ = newexpr(boolexpr_e);
                                                    $$->truelist = merge($1->truelist, $4->truelist);
                                                    $$->falselist = $4->falselist;
} 
    | term                                    {  $$ = $1; DEBUG_REDUCE("expr -> term"); $$ = $1; }
    ;


term: LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
        { $$ = $2; 
        // Pass the expr unchanged
        DEBUG_REDUCE("term -> (expr)"); }
    | MINUS expr%prec UMINUS {
        DEBUG_REDUCE("term -> -expr");
      
        $$ = newexpr(arithexpr_e);
        $$->sym = newtemp();
        emit(uminus, newexpr_constnum(0), $2, $$, 0, yylineno);}

    | NOT expr
        { DEBUG_REDUCE("term -> not expr");
        $2 = to_boolexpr($2);
        $$ = newexpr(boolexpr_e);
        $$->sym = newtemp();
        
        $$->truelist = $2->falselist;  // NOT swaps true and false lists
        $$->falselist = $2->truelist;
    }
    | PLUS_PLUS lvalue
        {temrs_error($2,"++");
        if($2->type == tableitem_e) {
													$$ = emit_iftableitem($2);
													emit(add, $$, newexpr_constnum(1), $$, nextquad(), yylineno);
													emit(add, $2, $2->index, $$, nextquad(), yylineno);
												}
									else {
													emit(add, $2, newexpr_constnum(1), $2,nextquad(), yylineno);
													$$ = newexpr(arithexpr_e);
													$$->sym = newtemp();
													emit(assign, $$, $2, NULL, nextquad(), yylineno);
												}
                                                 DEBUG_REDUCE("term -> ++lvalue"); }
    | lvalue PLUS_PLUS
        {temrs_error($1,"++"); 
     
        $$ = newexpr(arithexpr_e);
        $$->sym = newtemp();
        if ($1->type == tableitem_e) {
            $$ = emit_iftableitem($1); // Get current value
            expr* newval = newexpr(arithexpr_e);
            newval->sym = newtemp();
            emit(add, $$, newexpr_constnum(1), newval, 0, yylineno); // Compute new value
            emit(tablesetelem, $1, $1->index, newval, 0, yylineno); // Update table
        } else {
            emit(assign, $1, NULL, $$, 0, yylineno); // Copy current value
            emit(add, $1, newexpr_constnum(1), $1, 0, yylineno); // Update lvalue
        }
        DEBUG_REDUCE("term -> lvalue++"); }
    | MINUS_MINUS lvalue
        {temrs_error($2,"--"); 
        $$ = newexpr(arithexpr_e);
        $$->sym = newtemp();
        if ($2->type == tableitem_e) {
            expr* val = emit_iftableitem($2);
            emit(sub, val, newexpr_constnum(1), $$, 0, yylineno);
            emit(tablesetelem, $2, $2->index, $$, 0, yylineno);
        } else {
            emit(sub, $2, newexpr_constnum(1), $2, 0, yylineno);
            emit(assign, $2, NULL, $$, 0, yylineno);
        }
        DEBUG_REDUCE("term -> --lvalue"); }
    | lvalue MINUS_MINUS
        {temrs_error($1,"--"); 
        
        $$ = newexpr(arithexpr_e);
        $$->sym = newtemp();
        if ($1->type == tableitem_e) {
            $$ = emit_iftableitem($1);
            expr* newval = newexpr(arithexpr_e);
            newval->sym = newtemp();
            emit(sub, $$, newexpr_constnum(1), newval, 0, yylineno);
            emit(tablesetelem, $1, $1->index, newval, 0, yylineno);
        } else {
            emit(assign, $1, NULL, $$, 0, yylineno);
            emit(sub, $1, newexpr_constnum(1), $1, 0, yylineno);
        }
        DEBUG_REDUCE("term -> lvalue--"); }
    | primary
        { DEBUG_REDUCE("term -> primary"); 
        $$ = $1;}
    ;


assignexpr:
    lvalue ASSIGN expr
    {
        assign_error($1);
        DEBUG_REDUCE("assignexpr -> lvalue = expr");

        expr* rval = $3;

        // Convert boolean expressions to true/false if needed
        if (rval->type == boolexpr_e) {
            rval = boolify_expr(rval);
        }

        if ($1->type == tableitem_e) {
            // Handle table elements (unchanged)
            emit(tablesetelem, $1, $1->index, rval, -1, yylineno);
            $$ = emit_iftableitem($1);
            $$->type = assignexpr_e;
        } else {
            // Assign rval to lvalue
            emit(assign, rval, NULL, $1, 0, yylineno);
            // Create a temporary to hold lvalue's value
            expr* temp = newexpr(var_e);
            temp->sym = newtemp();
            emit(assign, temp, $1, NULL, 0, yylineno);
            $$ = temp;
        }
    };

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
                                expr* funcToCall = $1;
                                if($2->method){
                                    expr* self = newexpr(var_e);
                                    self = member_item($1, $2->name);
                                    self->next = $2->elist;
                                    $2->elist = self;
                                }

                                $$ = call_handler(funcToCall, $2->elist);
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
      /* empty */                           {$$ = nullptr; DEBUG_REDUCE("elist -> empty"); }
    | expr elist_tail                       {$1->next = $2; $$ = $1; DEBUG_REDUCE("elist -> expr"); }
    ;
elist_tail:
      COMMA expr elist_tail {
          $2->next = $3;
          $$ = $2;
      }
    | /* empty */ {
          $$ = nullptr;
      }
    ;

objectdef:
      LEFT_BRACKET elist RIGHT_BRACKET         { DEBUG_REDUCE("objectdef -> {elist}"); }
      | LEFT_BRACKET indexed RIGHT_BRACKET         { DEBUG_REDUCE("objectdef -> indexed "); }
    ;

indexed:
      indexedelem                           {  DEBUG_REDUCE("indexed -> indexedelem"); }
    | indexed COMMA indexedelem            {  DEBUG_REDUCE("indexed -> indexed , indexedelem"); }
    ;

indexedelem:
      LEFT_BRACE expr COLON expr RIGHT_BRACE
                                           { DEBUG_REDUCE("indexedelem -> [expr : expr]"); }
    ;

block:
      LEFT_BRACE { scope++; } stmt_list RIGHT_BRACE {$$=$3; exit_block(); DEBUG_REDUCE("block -> {stmt_list}"); }
    ;
    

funcdef:
      FUNCTION {enter_func(0, "");}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS {scope--;add_anon_function(args);}
            block {exit_func(0, "", $7->returnList); DEBUG_REDUCE("funcdef -> function(idlist) block"); }
                                          
    | FUNCTION IDENT {enter_func(1, *$2);}
            LEFT_PARENTHESIS idlist RIGHT_PARENTHESIS 
            {scope--; add_function(*$2, args);}
            block
            {exit_func(1, *$2, $8->returnList); DEBUG_REDUCE("funcdef -> function IDENT(idlist) block"); }
    ;


const:
      INTEGER     { $$ = newexpr(constnum_e); $$->numConst = $1;  DEBUG_REDUCE("const -> INTEGER"); }
    | REALCONST   { $$ = newexpr(constnum_e); $$->numConst = $1;  DEBUG_REDUCE("const -> REALCONST"); }
    | STRING      { $$ = newexpr_conststring(*$1); DEBUG_REDUCE("const -> STRING"); }
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

loopstart :                        {++loopcounter;}
          ;
loopend   :                        {--loopcounter;}
          ;
loopstmt  : loopstart stmt loopend {$$ = $2;}
          ;

whilestart : WHILE {
   
    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();
}
;

whilecond : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS {
   
    emit(if_eq, $2, newexpr_bool('1'), NULL, nextquad()+2, yylineno); //assign true
    

    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();
  
    emit(jump, NULL, NULL, 0,0,yylineno);
    
}
;

whilestmt:
    whilestart whilecond loopstmt {

        make_stmt($3);

        emit(jump, NULL, NULL, $1,(int)$1->numConst, yylineno);
        
        patchlabel((int)$2->numConst, nextquad());

       
        patchlist($3->breakList, nextquad());
        
        patchlist($3->contList, nextquad());
        
    }
;

N : {
    $$ = newexpr(constnum_e);
    $$->numConst = nextquad();
    emit(jump,NULL,NULL,0,-1,yylineno);
}
;

M : {
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
            emit(ret, nullptr, nullptr, nullptr, 0, nextquadlabel());
            $$ = new stmt_t();
            make_stmt($$);
            $$->returnList = nextquadlabel() - 1;
            DEBUG_REDUCE("returnstmt -> return ;");
        }
    | RETURN expr SEMICOLON
        {
            emit(ret, $2, nullptr, nullptr, 0, nextquadlabel());
            $$ = new stmt_t();
            make_stmt($$);
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
    return 0;
}

void yyerror(const char *s) {
    std::cerr << "Parse error: " << s << " at line : "<<yylineno <<" before token: " <<  yytext << endl;
}
