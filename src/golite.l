%{
#include <stdio.h>
#include <string.h>

#include "parser.tab.h"
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

extern int verbose_mode;

int lastToken = -1;
#define RETURN(token) lastToken = token; return token;

int needsSemiColon()
{	
	return lastToken == tIDENTIFIER || lastToken == tINTCONST || lastToken == tFLOATCONST || lastToken == tRUNECONST || lastToken == tSTRINGCONST || lastToken == tBREAK || lastToken == tCONTINUE || lastToken == tFALLTHROUGH || lastToken == tRETURN || lastToken == tINC || lastToken == tDEC || lastToken == ')' || lastToken == ']' || lastToken == '}' ;
}

%}

SPACE [ ]
ALPHANUMS [a-zA-Z0-9]
SYMBOLS [~@#$%&*+\/`<>=_|'.,;:!?{}\[\]\(\)^-]
SYMBOLS2 [~@#$%&*+\/`<>=_|.,;:!?{}\[\]\(\)^-]
ESCAPE [abfnrtv''\\"]

%option yylineno
%option noinput
%option nounput

%x block_comment
%s unfinished_comment

%%

[ \t\r]+ 				/* ignore */
\n 				{ if (needsSemiColon()) { if (verbose_mode) { printf(";\n"); } RETURN(';'); } }

break 				{ if (verbose_mode) printf("tBREAK\n"); RETURN(tBREAK); }
case  				{ if (verbose_mode) printf("tCASE\n"); RETURN(tCASE); }
chan     			{ if (verbose_mode) printf("tCHAN\n"); RETURN(tCHAN); }
const 				{ if (verbose_mode) printf("tCONST\n"); RETURN(tCONST); }
continue 			{ if (verbose_mode) printf("tCONTINUE\n"); RETURN(tCONTINUE); }
default 			{ if (verbose_mode) printf("tDEFAULT\n"); RETURN(tDEFAULT); }
defer 				{ if (verbose_mode) printf("tDEFER\n"); RETURN(tDEFER); }
else 				{ if (verbose_mode) printf("tELSE\n"); RETURN(tELSE); }
fallthrough 			{ if (verbose_mode) printf("tFALLTHROUGH\n"); RETURN(tFALLTHROUGH); }
for 				{ if (verbose_mode) printf("tFOR\n"); RETURN(tFOR); }
func 				{ if (verbose_mode) printf("tFUNC\n"); RETURN(tFUNC); }
go 				{ if (verbose_mode) printf("tGO\n"); RETURN(tGO); }
goto 				{ if (verbose_mode) printf("tGOTO\n"); RETURN(tGOTO); }
if 				{ if (verbose_mode) printf("tIF\n"); RETURN(tIF); }
import 				{ if (verbose_mode) printf("tIMPORT\n"); RETURN(tIMPORT); }
interface 			{ if (verbose_mode) printf("tINTERFACE\n"); RETURN(tINTERFACE); }
map 				{ if (verbose_mode) printf("tMAP\n"); RETURN(tMAP); }
package				{ if (verbose_mode) printf("tPACKAGE\n"); RETURN(tPACKAGE); }
range 				{ if (verbose_mode) printf("tRANGE\n"); RETURN(tRANGE); } 
return 				{ if (verbose_mode) printf("tRETURN\n"); RETURN(tRETURN); }
select 				{ if (verbose_mode) printf("tSELECT\n"); RETURN(tSELECT); }
struct 				{ if (verbose_mode) printf("tSTRUCT\n"); RETURN(tSTRUCT); }
switch 				{ if (verbose_mode) printf("tSWITCH\n"); RETURN(tSWITCH); }
type 				{ if (verbose_mode) printf("tTYPE\n"); RETURN(tTYPE); }
var 				{ if (verbose_mode) printf("tVAR\n"); RETURN(tVAR); }
print 				{ if (verbose_mode) printf("tPRINT\n"); RETURN(tPRINT); } 
println 			{ if (verbose_mode) printf("tPRINTLN\n"); RETURN(tPRINTLN); }
append 				{ if (verbose_mode) printf("tAPPEND\n"); RETURN(tAPPEND); }
"^"				{ if (verbose_mode) printf("^\n"); RETURN('^'); }
"&^"				{ if (verbose_mode) printf("&^\n"); RETURN(tANDXOR); }
"&^="				{ if (verbose_mode) printf("&^=\n"); RETURN(tSANDXOR); }
"+"				{ if (verbose_mode) printf("+\n"); RETURN('+'); }
"&"				{ if (verbose_mode) printf("&\n"); RETURN('&'); }
"+="				{ if (verbose_mode) printf("+=\n"); RETURN(tSPLUS); }
"&="				{ if (verbose_mode) printf("&=\n"); RETURN(tSAND); }
"&&"				{ if (verbose_mode) printf("&&\n"); RETURN(tAND); }
"=="				{ if (verbose_mode) printf("==\n"); RETURN(tEQ); }
"!="				{ if (verbose_mode) printf("!=\n"); RETURN(tNEQ); }
"("				{ if (verbose_mode) printf("(\n"); RETURN('('); }
")"				{ if (verbose_mode) printf(")\n"); RETURN(')'); }
"-"				{ if (verbose_mode) printf("-\n"); RETURN('-'); }
"|"				{ if (verbose_mode) printf("|\n"); RETURN('|'); }
"-="				{ if (verbose_mode) printf("-=\n"); RETURN(tSMINUS); }
"|="				{ if (verbose_mode) printf("|=\n"); RETURN(tSOR); }
"||"				{ if (verbose_mode) printf("||\n"); RETURN(tOR); }
"<"				{ if (verbose_mode) printf("<\n"); RETURN('<'); }
"<="				{ if (verbose_mode) printf("<=\n"); RETURN(tLEQ); }
"["				{ if (verbose_mode) printf("[\n"); RETURN('['); }
"]"				{ if (verbose_mode) printf("]\n"); RETURN(']'); }
"*"				{ if (verbose_mode) printf("*\n"); RETURN('*'); }
"*="				{ if (verbose_mode) printf("*=\n"); RETURN(tSMULTIPLY); }
"^="				{ if (verbose_mode) printf("^=\n"); RETURN(tSXOR); }
"<-"				{ if (verbose_mode) printf("<-\n"); RETURN(tARROW); }
">"				{ if (verbose_mode) printf(">\n"); RETURN('>'); }
">="				{ if (verbose_mode) printf(">=\n"); RETURN(tGEQ); }
"{"				{ if (verbose_mode) printf("{\n"); RETURN('{'); }
"}"				{ if (verbose_mode) printf("}\n"); RETURN('}'); }
"/"				{ if (verbose_mode) printf("/\n"); RETURN('/'); }
"<<"				{ if (verbose_mode) printf("<<\n"); RETURN(tLSHIFT); }
"/="				{ if (verbose_mode) printf("/=\n"); RETURN(tSDIVIDE); }
"<<="				{ if (verbose_mode) printf("<<=\n"); RETURN(tSLSHIFT); }
"++"				{ if (verbose_mode) printf("++\n"); RETURN(tINC); }
"="				{ if (verbose_mode) printf("=\n"); RETURN('='); }
":="				{ if (verbose_mode) printf(":=\n"); RETURN(tDOTASSIGN); }
","				{ if (verbose_mode) printf(",\n"); RETURN(','); }
";"				{ if (verbose_mode) printf(";\n"); RETURN(';'); }
"%"				{ if (verbose_mode) { printf("%%"); printf("\n"); } RETURN('%'); }
">>"				{ if (verbose_mode) printf(">>\n"); RETURN(tRSHIFT); }
"%="				{ if (verbose_mode) { printf("%%="); printf("\n"); } RETURN(tSMOD); }
">>="				{ if (verbose_mode) printf(">>=\n"); RETURN(tSRSHIFT); }
"--"				{ if (verbose_mode) printf("--\n"); RETURN(tDEC); } 
"!"				{ if (verbose_mode) printf("!\n"); RETURN('!'); } 
"..."				{ if (verbose_mode) printf("...\n"); RETURN(tPOINTS); }
"."				{ if (verbose_mode) printf(".\n"); RETURN('.'); }
":"				{ if (verbose_mode) printf(":\n"); RETURN(':'); }
[/]{2}.*			/* ignore */

"/*"			{ BEGIN(block_comment); }
<block_comment>"*/" 	{ BEGIN(INITIAL); }
<block_comment>\n   	{ }
<block_comment>.     	{ }




([0-9]+[.][0-9]*|[.][0-9]+)	{ if (verbose_mode) printf("tFLOATCONST(%s)\n", yytext); yylval.floatconst = strdup(yytext); RETURN(tFLOATCONST); }
0|([1-9][0-9]*) 		{ if (verbose_mode) printf("tINTCONST(%s)\n", yytext); yylval.intconst = atoi(yytext); RETURN(tINTCONST); }

[']({SPACE}|{ALPHANUMS}|({SYMBOLS2})|\\{ESCAPE}|([\r][\n])|[\n\t]){1}['] { if (verbose_mode) printf("tRUNECONST(%s)\n", yytext); yylval.runeconst = strdup(yytext); RETURN(tRUNECONST); }

[a-zA-Z_][a-zA-Z0-9_]* 	{ yylval.idconst = strdup(yytext); if (verbose_mode) printf("tIDENTIFIER(%s)\n", yytext); RETURN(tIDENTIFIER); }

(["]({SPACE}|{ALPHANUMS}|{SYMBOLS}|\\{ESCAPE})*["])|([`]({SPACE}|{ALPHANUMS}|{SYMBOLS}|\\{ESCAPE})*[`]) { yylval.stringconst = strdup(yytext); if (verbose_mode) printf("tSTRINGCONST(%s)\n", yytext); RETURN(tSTRINGCONST); }

<<EOF>> {  if (YY_START == block_comment) { printf("Error: unclosed block comment.\n"); exit(1); } if (needsSemiColon()) { if (verbose_mode) printf(";\n"); RETURN(';'); } yyterminate(); }

.       { fprintf(stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); }

%%