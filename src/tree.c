#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "tree.h"

extern int yylineno;

/* NOTE:
 * The following are functions to helper build the abstract syntax tree.
 */

// Program function
PROGRAM *makePROGRAM(PACKAGE *package, DECLARATION *declarations)
{
	PROGRAM *p = malloc(sizeof(PROGRAM));
	p->package = package;
	p->declarations = declarations;
	return p;
}

// Package function
PACKAGE *makePACKAGE(char *name)
{	
	PACKAGE *p = malloc(sizeof(PACKAGE));
	p->name = name;
	p->lineno = yylineno;
	return p;
}

// Declaration functions
DECLARATION *makeDECLARATION_variable(IDL *ids, TYPE *type, EXP *expls, DECLARATION *next, int format)
{
	DECLARATION *d = malloc(sizeof(DECLARATION));
	d->lineno = yylineno;
	d->kind = k_declarationVariable;
	d->val.variableD.ids = ids;
	d->val.variableD.type = type; 
	d->val.variableD.expls = expls;
	d->val.variableD.format = format;
	// For weeding
	if (d->val.variableD.format == 1) {
		d->val.variableD.expressionAssigned = 0;
	}
	else {
		d->val.variableD.expressionAssigned = 1;
	}
	d->next = next;
	return d;
}

DECLARATION *makeDECLARATION_type(IDL *ids, TYPE *type, DECLARATION *next)
{
	DECLARATION *d = malloc(sizeof(DECLARATION));
	d->lineno = yylineno;
	d->kind = k_declarationType;
	d->val.typeD.ids = ids;
	d->val.typeD.type = type;
	d->next = next;
	return d;
}

DECLARATION *makeDECLARATION_function(char *name, FORMAL *args, TYPE *returnType, STATEMENT *body, DECLARATION *next)
{
	DECLARATION *d = malloc(sizeof(DECLARATION));
	d->lineno = yylineno;
	d->kind = k_declarationFunction;
	d->val.functionD.id = name;
	d->val.functionD.arguments = args;
	d->val.functionD.returnType = returnType;
	d->val.functionD.body = body;
	d->next = next;
	return d;
}

// Formal function
FORMAL *makeFORMAL(char *name, TYPE *type, FORMAL *next)
{
	FORMAL *f = malloc(sizeof(FORMAL));
	f->lineno = yylineno;
	f->name = name;
	f->type = type;
	f->next = next;
	return f;
}

// Identifier list function
IDL *makeIDENTIFIERLIST(char *name, IDL *nextIdentifier) 
{
	IDL *i = malloc(sizeof(IDL));
	i->lineno = yylineno;
	i->name = name;
	i->next = nextIdentifier;
	return i;
}

// Type functions
TYPE *makeTYPE_ref(char *name)
{
	TYPE *t = malloc(sizeof(TYPE));
	t->lineno = yylineno;
	t->kind = k_typeReference;
	t->val.refT.id = name;
	return t;
}	

TYPE *makeTYPE_ARRAY(int value, TYPE *type)
{
	TYPE *t = malloc(sizeof(TYPE));
	t->lineno = yylineno;
	t->kind = k_typeArray;
	t->val.arrayT.value = value;
	t->val.arrayT.type = type;
	return t;
}

TYPE *makeTYPE_parenthesis(TYPE *type) 
{
	TYPE *t = malloc(sizeof(TYPE));
	t->lineno = yylineno;
	t->kind = k_typeParenthesis;
	t->val.parT.type = type;
	return t;
}

// Statement functions
STATEMENT *makeSTATEMENT_block(STATEMENT *body, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementBlock;
	s->val.blockS.body = body;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_expression(EXP *exp, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementExpression;
	s->val.expressionS.exp = exp;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_assignment(EXP *ids, EXP *exp, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementAssignment;
	s->val.assignmentS.ids = ids;
	s->val.assignmentS.exp = exp;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_increment(EXP *exp, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementIncrement;
	s->val.incrementS.exp = exp;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_decrement(EXP *exp, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementDecrement;
	s->val.decrementS.exp = exp;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_print(EXP *printlist, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementPrint;
	s->val.printS.printlist = printlist;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_println(EXP *printlist, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementPrintln;
	s->val.printlnS.printlist = printlist;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_return(EXP *exp, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementReturn;
	s->val.returnS.exp = exp;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_if(EXP *condition, STATEMENT *body, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementIf;
	s->val.ifS.condition = condition;
	s->val.ifS.body = body;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_ifelse(EXP *condition, STATEMENT *thenPart, STATEMENT *elsePart, STATEMENT *nextIf, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementIfElse;
	s->val.ifelseS.condition = condition;
	s->val.ifelseS.thenPart = thenPart;
	s->val.ifelseS.elsePart = elsePart;
	s->nextIf = nextIf;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_switch(EXP *condition, STATEMENT *clauses, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;	
	s->kind = k_statementSwitch;
	s->val.switchS.condition = condition;
	s->val.switchS.clauses = clauses;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_switchcase(EXP *expressionList, STATEMENT *content, STATEMENT *nextClause)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;
	s->kind = k_statementSwitchCase;
	s->val.caseclauseS.explist = expressionList; 	
	s->val.caseclauseS.content = content;
	s->nextClause = nextClause;
	s->next = NULL;
	return s;
}

STATEMENT *makeSTATEMENT_switchdefault(STATEMENT *content, STATEMENT *nextClause)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;	
	s->kind = k_statementSwitchDefault;
	s->val.defaultclauseS.content = content;
	s->nextClause = nextClause;
	s->next = NULL;
	return s;
}

STATEMENT *makeSTATEMENT_for(STATEMENT *initStmt, EXP *condition, STATEMENT *postStmt, STATEMENT *body, STATEMENT *next, int keepSemiColon)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;	
	s->kind = k_statementFor;
	s->val.forS.initStmt = initStmt;
	s->val.forS.condition = condition;
	s->val.forS.postStmt = postStmt;
	s->val.forS.body = body;
	s->next = next;

	if (initStmt == NULL && condition == NULL && postStmt == NULL) {
		s->val.forS.infinitel = 1;
	}

	if (initStmt != NULL && condition != NULL && postStmt != NULL) {
		s->val.forS.threel = 1;
	}

	if (initStmt != NULL && condition != NULL && postStmt == NULL) {
		s->val.forS.threel = 1;
	}

	if (initStmt == NULL && condition != NULL && postStmt != NULL) {
		s->val.forS.threel = 1;
	}

	if (initStmt != NULL && condition == NULL && postStmt != NULL) {
		s->val.forS.threel = 1;
	}


	if (initStmt == NULL && condition != NULL && postStmt == NULL) {
		s->val.forS.threel = 1;
	}

	if (initStmt != NULL && condition == NULL && postStmt == NULL) {
		s->val.forS.threel = 1;
	}

	if (initStmt == NULL && condition == NULL && postStmt != NULL) {
		s->val.forS.threel = 1;
	}

	if (initStmt == NULL && condition != NULL && postStmt == NULL) {
		if (keepSemiColon) {
			s->val.forS.threel = 1;
		}
		else {
			s->val.forS.whilel = 1;
		}
	}

	return s;
}

STATEMENT *makeSTATEMENT_break(STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;	
	s->kind = k_statementBreak;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_continue(STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;	
	s->kind = k_statementContinue;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_declaration(DECLARATION *d, STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;	
	s->kind = k_statementDeclaration;
	s->val.declS.decl = d;
	s->next = next;
	return s;
}

STATEMENT *makeSTATEMENT_skip(STATEMENT *next)
{
	STATEMENT *s = malloc(sizeof(STATEMENT));
	s->lineno = yylineno;	
	s->kind = k_statementSkip;
	s->next = next;
	return s;
}

// Expression functions
EXP *makeEXP_identifier(char *name)
{	
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionIdentifier;
	e->val.identifierE.id = name;
	return e;
}

EXP *makeEXP_parenthesis(EXP *body)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionParenthesis;
	e->val.parenthesisE.body = body;
	return e;
}

EXP *makeEXP_intconst(int n)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionInt;
	e->val.intconst = n;
	return e;
}

EXP *makeEXP_boolconst(int n)
{	
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionBool;
	e->val.boolconst = n;
	return e;
}

EXP *makeEXP_floatconst(char *n)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionFloat64;
	e->val.floatconst = n;
	return e;
}

EXP *makeEXP_stringconst(char *n)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionString;
	e->val.stringconst = n;
	return e;
}

EXP *makeEXP_runeconst(char *n)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionRune;
	e->val.stringconst = n;
	return e;
}

EXP *makeEXP_uplus(EXP *body)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionUplus;
	e->val.uplusE.body = body;
	return e;
}

EXP *makeEXP_uminus(EXP *body)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionUminus;
	e->val.uminusE.body = body;
	return e;
}

EXP *makeEXP_unot(EXP *body)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionUnot;
	e->val.unotE.body = body;
	return e;
}

EXP *makeEXP_uxor(EXP *body)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionUxor;
	e->val.uxorE.body = body;
	return e;
}

EXP *makeEXP_tor(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionTor;
	e->val.torE.left = left;
	e->val.torE.right = right;
	return e;
}

EXP *makeEXP_tand(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionTand;
	e->val.tandE.left = left;
	e->val.tandE.right = right;
	return e;
}

EXP *makeEXP_eq(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionEq;
	e->val.eqE.left = left;
	e->val.eqE.right = right;
	return e;
}

EXP *makeEXP_neq(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionNeq;
	e->val.neqE.left = left;
	e->val.neqE.right = right;
	return e;
}

EXP *makeEXP_l(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionL;
	e->val.lE.left = left;
	e->val.lE.right = right;
	return e;
}

EXP *makeEXP_leq(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionLeq;
	e->val.leqE.left = left;
	e->val.leqE.right = right;
	return e;
}

EXP *makeEXP_g(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionG;
	e->val.gE.left = left;
	e->val.gE.right = right;
	return e;
}

EXP *makeEXP_geq(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionGeq;
	e->val.geqE.left = left;
	e->val.geqE.right = right;
	return e;
}

EXP *makeEXP_plus(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionPlus;
	e->val.plusE.left = left;
	e->val.plusE.right = right;
	return e;
}

EXP *makeEXP_minus(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionMinus;
	e->val.minusE.left = left;
	e->val.minusE.right = right;
	return e;
}

EXP *makeEXP_or(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionOr;
	e->val.orE.left = left;
	e->val.orE.right = right;
	return e;
}

EXP *makeEXP_xor(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionXor;
	e->val.xorE.left = left;
	e->val.xorE.right = right;
	return e;
}

EXP *makeEXP_multiply(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionMultiply;
	e->val.multiplyE.left = left;
	e->val.multiplyE.right = right;
	return e;
}

EXP *makeEXP_divide(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionDivide;
	e->val.divideE.left = left;
	e->val.divideE.right = right;
	return e;
}

EXP *makeEXP_mod(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionMod;
	e->val.modE.left = left;
	e->val.modE.right = right;
	return e;
}

EXP *makeEXP_lshift(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionLshift;
	e->val.lshiftE.left = left;
	e->val.lshiftE.right = right;
	return e;
}

EXP *makeEXP_rshift(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionRshift;
	e->val.rshiftE.left = left;
	e->val.rshiftE.right = right;
	return e;
}

EXP *makeEXP_and(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionAnd;
	e->val.andE.left = left;
	e->val.andE.right = right;
	return e;
}

EXP *makeEXP_nand(EXP *left, EXP *right)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionNand;
	e->val.nandE.left = left;
	e->val.nandE.right = right;
	return e;
}

EXP *makeEXP_functionCall(char *name, EXP *expressionList)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionFunctionCall;
	e->val.funcE.id = name;
	e->val.funcE.explist = expressionList;
	return e;
}

EXP *makeEXP_array(EXP *exp, EXP *value)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionArray;
	e->val.arrayE.exp = exp;
	e->val.arrayE.value = value;
	return e;
}

/* Helper data structure (linked list)  */

void insertFirst(int key, char* data) {
  	
	struct node *newNode, *temp;

    newNode = (struct node*)malloc(sizeof(struct node));

    newNode->data = data; 
    newNode->key = key;
    newNode->next = NULL; 

    if (head_inits == NULL) {

    	head_inits = newNode;
    }
    else {
    	temp = head_inits;

	    while (temp->next != NULL) temp = temp->next;
	    temp->next = newNode;
    }
}

void printLL() {

   struct node *ptr = head_inits;
   printf("\n[ ");
	
   while (ptr != NULL) {
      printf("(%d,%s) ", ptr->key, ptr->data);
      ptr = ptr->next;
   }
	
   printf(" ]\n");
}




