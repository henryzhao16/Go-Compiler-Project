%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"

extern int yylineno;

extern PROGRAM *theprogram;

int yylex();

void yyerror(const char *s) { fprintf(stderr, "Error: (line %d) %s\n", yylineno, s); exit(1); }
%}

%locations
%error-verbose

%code requires
{
	#include "tree.h"	
}

%union {
    struct PROGRAM *program;
    struct PACKAGE *package;
    struct DECLARATION *declaration;
    struct TYPE *type;
    struct FORMAL *formal;
    struct STATEMENT *statement;
    struct EXP *exp;
    struct IDL *idl;
    int intconst;
    int boolconst;
    char *floatconst;
    char *stringconst;
    char *idconst;	
    char *runeconst;
}

%type <program> program
%type <package> package 
%type <declaration> declarations declaration varDeclaration typeDeclaration funcDeclaration
%type <type> type opttype
%type <formal> formals neformals formal
%type <statement> statements statement 
%type <statement> simplestatement declarationStatement
%type <statement> ifstatement ifelsestatement ifstatements optifelsestatement
%type <statement> switchstatement forstatement switchstmts switchstmt
%type <statement> returnstatement
%type <exp> expression nexps exp optexp expid expcall expunaries expbinaries expliterals arrexp
%type <idl> identifierlist

%token tBREAK
%token tCASE	
%token tCHAN
%token tCONST
%token tCONTINUE
%token tDEFAULT
%token tDEFER
%token tELSE
%token tFALLTHROUGH
%token tFOR
%token tFUNC
%token tGO
%token tGOTO
%token tIF
%token tIMPORT
%token tINTERFACE
%token tMAP
%token tPACKAGE
%token tRANGE
%token tRETURN
%token tSELECT
%token tSTRUCT
%token tSWITCH
%token tTYPE
%token tVAR
%token tPRINT
%token tPRINTLN
%token tAPPEND
%token <intconst> tINTCONST
%token <floatconst> tFLOATCONST
%token <idconst> tIDENTIFIER
%token <boolconst> tBOOLCONST
%token <stringconst> tSTRINGCONST
%token <runeconst> tRUNECONST
%token tSPLUS
%token tSAND
%token tAND
%token tEQ
%token tNEQ
%token tSMINUS
%token tSOR
%token tOR
%token tLEQ
%token tSMULTIPLY
%token tSXOR
%token tARROW
%token tGEQ
%token tLSHIFT
%token tSDIVIDE
%token tSLSHIFT
%token tINC
%token tDOTASSIGN
%token tRSHIFT
%token tSMOD
%token tSRSHIFT
%token tDEC
%token tPOINTS
%token tANDXOR
%token tSANDXOR

%left tOR
%left tAND
%left tEQ tNEQ '<' tLEQ '>' tGEQ
%left '+' '-' '|' '^'    
%left '*' '/' '%' tLSHIFT tRSHIFT '&' tANDXOR 
%left UPLUS UMINUS UNOT UXOR

%start program

%%

program: package declarations
         { theprogram = makePROGRAM($1,$2); }
;

package: tPACKAGE tIDENTIFIER ';'
         { $$ = makePACKAGE($2); }
;

declarations: /* empty */
              { $$ = NULL; }
            | declaration ';' declarations
              { $$ = $1; $$->next = $3; }
;

declaration: varDeclaration
             { $$ = $1; }
           | typeDeclaration
             { $$ = $1; }
           | funcDeclaration
             { $$ = $1; }
;

varDeclaration: tVAR identifierlist type
                { $$ = makeDECLARATION_variable($2,$3,NULL,NULL,1); }
              | tVAR identifierlist '=' expression
                { $$ = makeDECLARATION_variable($2,NULL,$4,NULL,2); }
              | tVAR identifierlist type '=' expression
                { $$ = makeDECLARATION_variable($2,$3,$5,NULL,3); }
;

typeDeclaration: tTYPE identifierlist type
                 { $$ = makeDECLARATION_type($2,$3,NULL); }
;

funcDeclaration: tFUNC tIDENTIFIER '(' formals ')' opttype '{' statements '}' 
                 { $$ = makeDECLARATION_function($2,$4,$6,$8,NULL); }
;

identifierlist: tIDENTIFIER
                { $$ = makeIDENTIFIERLIST($1,NULL); }
              | identifierlist ',' tIDENTIFIER 
                { $$ = makeIDENTIFIERLIST($3,NULL); $$->next = $1; }
;

type: tIDENTIFIER
      { $$ = makeTYPE_ref($1); }
    | '[' tINTCONST ']' type
      { $$ = makeTYPE_ARRAY($2,$4); }
    | '(' type ')'
      { $$ = makeTYPE_parenthesis($2); }
;

opttype: /* empty */
         { $$ = NULL; }
       | type
         { $$ = $1; }
;

formals: /* empty */
         { $$ = NULL; }
       | neformals
         { $$ = $1; }
;

neformals: formal
           { $$ = $1; }
         | neformals ',' formal
           { $$ = $3; $$->next = $1; }
;

formal: tIDENTIFIER type
        { $$ = makeFORMAL($1,$2,NULL); } 
;

statements: /* empty */
            { $$ = NULL; }
          | statement ';' statements
            { if ($1 == NULL) { $$ = $3; } else { $$ = $1; $$->next = $3; } }
;

statement: simplestatement 
           { $$ = $1; }
         | declarationStatement
           { $$ = $1; }
         | ifstatements
           { $$ = $1; }
         | switchstatement
           { $$ = $1; }
         | forstatement
           { $$ = $1; }
         | '{' statements '}'
            { $$ = makeSTATEMENT_block($2,NULL); }
;

ifstatements: ifstatement
              { $$ = $1; }
            | ifelsestatement
              { $$ = $1; }
;

declarationStatement: varDeclaration
                      { $$ = makeSTATEMENT_declaration($1,NULL); }
                    | typeDeclaration
                      { $$ = makeSTATEMENT_declaration($1,NULL); }
;
           
simplestatement: /* empty */
                 { $$ = NULL; }
               | expression '=' expression
                 { $$ = makeSTATEMENT_assignment($1,$3,NULL); }
               | exp tINC
                 { $$ = makeSTATEMENT_increment($1,NULL); }
               | exp tDEC
                 { $$ = makeSTATEMENT_decrement($1,NULL); }
               | tPRINT '(' expression ')'
                 { $$ = makeSTATEMENT_print($3,NULL); }
               | tPRINTLN '(' expression ')'
                 { $$ = makeSTATEMENT_println($3,NULL); }
               | returnstatement
                 { $$ = $1; }
               | tBREAK 
                 { $$ = makeSTATEMENT_break(NULL); }
               | tCONTINUE
                 { $$ = makeSTATEMENT_continue(NULL); }
               | exp 
                 { $$ = makeSTATEMENT_expression($1,NULL); }
;

returnstatement: tRETURN 
                 { $$ = makeSTATEMENT_return(NULL,NULL); }
               | tRETURN exp
                 { $$ = makeSTATEMENT_return($2,NULL); }
;

ifstatement: tIF exp '{' statements '}' 
             { $$ = makeSTATEMENT_if($2,$4,NULL); }
;

ifelsestatement: tIF exp '{' statements '}' tELSE '{' statements '}'
                 { $$ = makeSTATEMENT_ifelse($2,$4,$8,NULL,NULL); }
               | tIF exp '{' statements '}' tELSE optifelsestatement
                 { $$ = makeSTATEMENT_ifelse($2,$4,NULL,$7,NULL); }
;         

optifelsestatement: ifstatement
                    { $$ = $1; }
                  | ifelsestatement
                    { $$ = $1; }

switchstatement: tSWITCH optexp '{' switchstmts '}'
                 { $$ = makeSTATEMENT_switch($2,$4,NULL); }
;          

optexp: /* empty */
        { $$ = NULL; }
      | exp  
        { $$ = $1; }
;

switchstmts: /* empty */
             { $$ = NULL; }
           | switchstmt switchstmts
             { $$ = $1; $$->nextClause = $2; } 
;

switchstmt: tCASE nexps ':' statements
             { $$ = makeSTATEMENT_switchcase($2,$4,NULL); }
           | tDEFAULT ':' statements
             { $$ = makeSTATEMENT_switchdefault($3,NULL); }
;

forstatement: tFOR simplestatement ';' expression ';' simplestatement '{' statements '}'
              { $$ = makeSTATEMENT_for($2,$4,$6,$8,NULL,1); }
            | tFOR optexp '{' statements '}'
              { $$ = makeSTATEMENT_for(NULL,$2,NULL,$4,NULL,0); }
;

expression : /* empty */
             { $$ = NULL; }
           | nexps
             { $$ = $1; }
;

nexps: exp 
       { $$ = $1; }
     | nexps ',' exp
       { $$ = $3; $$->next = $1; }    
;

exp: expliterals
     { $$ = $1; }
   | expid
     { $$ = $1; }
   | expcall 
     { $$ = $1; }
   | expunaries
     { $$ = $1; }
   | expbinaries
     { $$ = $1; }
   | '(' exp ')'
     { $$ = makeEXP_parenthesis($2); }
   | arrexp 
     { $$ = $1; }
;

arrexp: expid '[' exp ']'
        { $$ = makeEXP_array($1,$3); }
      | arrexp '[' exp ']'
        { $$ = makeEXP_array($1,$3); }
;

expid: tIDENTIFIER
       { $$ = makeEXP_identifier($1); }
;

expliterals: tINTCONST
             { $$ = makeEXP_intconst($1); }
           | tBOOLCONST
             { $$ = makeEXP_boolconst($1); }
           | tFLOATCONST
             { $$ = makeEXP_floatconst($1); }
           | tSTRINGCONST
             { $$ = makeEXP_stringconst($1); }
           | tRUNECONST
             { $$ = makeEXP_runeconst($1); }
;

expunaries: '+' exp %prec UPLUS
            { $$ = makeEXP_uplus($2); }
          | '-' exp %prec UMINUS
            { $$ = makeEXP_uminus($2); }
          | '!' exp %prec UNOT
            { $$ = makeEXP_unot($2); }
          | '^' exp %prec UXOR
            { $$ = makeEXP_uxor($2); }
;

expcall: tIDENTIFIER '(' expression ')'
         { $$ = makeEXP_functionCall($1,$3); }
;

expbinaries: exp tOR exp
             { $$ = makeEXP_tor($1,$3); }
           | exp tAND exp
             { $$ = makeEXP_tand($1,$3); }
           | exp tEQ exp
             { $$ = makeEXP_eq($1,$3); }
           | exp tNEQ exp
             { $$ = makeEXP_neq($1,$3); }
           | exp '<' exp
             { $$ = makeEXP_l($1,$3); }
           | exp tLEQ exp
             { $$ = makeEXP_leq($1,$3); }
           | exp '>' exp
             { $$ = makeEXP_g($1,$3); }
           | exp tGEQ exp
             { $$ = makeEXP_geq($1,$3); }
           | exp '+' exp
             { $$ = makeEXP_plus($1,$3); }
           | exp '-' exp
             { $$ = makeEXP_minus($1,$3); }
           | exp '|' exp
             { $$ = makeEXP_or($1,$3); }
           | exp '^' exp
             { $$ = makeEXP_xor($1,$3); }
           | exp '*' exp
             { $$ = makeEXP_multiply($1,$3); }
           | exp '/' exp
             { $$ = makeEXP_divide($1,$3); }
           | exp '%' exp
             { $$ = makeEXP_mod($1,$3); }
           | exp tLSHIFT exp
             { $$ = makeEXP_lshift($1,$3); }
           | exp tRSHIFT exp
             { $$ = makeEXP_rshift($1,$3); }
           | exp '&' exp
             { $$ = makeEXP_and($1,$3); }
           | exp tANDXOR exp
             { $$ = makeEXP_nand($1,$3); }
;

%%
