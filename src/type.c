#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol.h"
#include "type.h"

/* NOTE:
 * The following are type check functions.
 */

void checkVariableDeclaration(TYPE *t, EXP *exps)
{	
	if (t != NULL && exps != NULL) {

		checkVariableDeclaration(t, exps->next);

		if (t->typeK != exps->typeK) {
			printf("Error: ill-typed declaration.\n");
			exit(1);
		}
		else {
			if (t->isArray != exps->isArray) {
				printf("Error: ill-typed array declaration.\n");
				exit(1);
			}
		}
	}
}

void checkArrayDeclaration(TYPE *t, EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;

	if (t != NULL && e != NULL) {

		if (e->kind == k_expressionIdentifier) {

			if (t->kind == k_typeArray) {

				s = getSymbol(sym, e->val.identifierE.id);

				if (s != NULL) {
					compareArrayType(t, s->type);
				}
			}
		}
	}
}

void compareArrayType(TYPE *t1, TYPE *t2)
{
	if (t1 != NULL && t2 != NULL) {

		if (t1->kind == k_typeArray && t2->kind == k_typeArray) {

			if (t1->val.arrayT.value != t2->val.arrayT.value) {

				printf("Error: ill-typed array declaration.\n");
				exit(1);
			}
			else {
				compareArrayType(t1->val.arrayT.type, t2->val.arrayT.type);
			}
		}
	}
	else if (t1 == NULL && t2 == NULL) {
		// IGNORE
	}
	else {
		printf("Error: ill-typed array declaration.\n");
		exit(1);
	}
}

void checkDeclarationFct(EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;

	if (e != NULL) {

		if (e->kind == k_expressionFunctionCall) {

			s = getSymbol(sym, e->val.funcE.id);

			if (s->kind == functionSym) {

				if (s->val.declarationS->val.functionD.returnType == NULL) {
					printf("Error: ill-typed declaration.\n");
					exit(1);
				}
			}
		}
	}
}

void checkTypeSymCast(TYPE *castType, EXP *e, SymbolTable *sym)
{	
	if (e != NULL && castType != NULL) {

		if (castType->typeK != e->typeK) {

			if (castType->typeK->base == intTYPE) {

				if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != runeTYPE) {
					printf("Error: illegal type casting.\n");
					exit(1);
				}
			} 
			else if (castType->typeK->base == float64TYPE) {

				if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != runeTYPE) {
					printf("Error: illegal type casting.\n");
					exit(1);
				}
			} 
			else if (castType->typeK->base == runeTYPE) {

				if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != runeTYPE) {
					printf("Error: illegal type casting.\n");
					exit(1);
				}
			} 
			else if (castType->typeK->base == boolTYPE) {

				if (e->typeK->base != boolTYPE) {
					printf("Error: illegal type casting.\n");
					exit(1);
				}
			} 
			else if (castType->typeK->base == stringTYPE) {

				if (e->typeK->base != intTYPE && e->typeK->base != stringTYPE && e->typeK->base != runeTYPE) {
					printf("Error: illegal type casting.\n");
					exit(1);
				}
			}
			else {
				printf("Error: illegal use of non-base type as a form of casting.\n");
				exit(1);
			}
		}
	}
}

void checkReturn(EXP *e, TYPE *t)
{	
	if (e != NULL && t != NULL) {
			
		if (e->typeK != t->typeK) {
			printf("Error: ill-typed return statement.\n");
			exit(1);
		}
	}
}

void checkAssignmentType(EXP *left, EXP *right, SymbolTable *sym)
{	
	SYMBOL *s1, *s2;

	if (left != NULL && right != NULL) {

		checkAssignmentType(left->next, right->next, sym);

		if (strcmp(left->val.identifierE.id, "_") != 0) {

			
			if (left->typeK != right->typeK) {
				printf("Error: ill-typed assignment statement.\n");
				exit(1);
			}	
			
			if (left->kind == k_expressionIdentifier && right->kind == k_expressionIdentifier) {
				
				s1 = getSymbol(sym, left->val.identifierE.id);
				s2 = getSymbol(sym, right->val.identifierE.id);

				if (s1 != NULL && s2 != NULL) {

					if (s1->type != NULL && s2->type != NULL && s1->type->kind == k_typeArray && s2->type->kind == k_typeArray) { 
						
						compareArrayType(s1->type, s2->type);
					}
				}
			}
		}
	}
 }

void checkPrint(EXP *e, SymbolTable *sym) 
{	
	SYMBOL *s; 

	if (e != NULL) {

		checkPrint(e->next, sym);

		if (e->kind == k_expressionIdentifier) {

			s = getSymbol(sym, e->val.identifierE.id);

			if (s != NULL) {

				if (s->type->isArray) {
					printf("Error: cannot print array struct.\n");
					exit(1);
				}
			}
		}

		if (e->kind == k_expressionFunctionCall) {

			s = getSymbol(sym, e->val.funcE.id);

			if (s != NULL) {

				if (s->val.declarationS->val.functionD.returnType == NULL) {
					printf("Error: cannot print function call without return type.\n");
					exit(1);
				}
			}
		}

		if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != boolTYPE && e->typeK->base != runeTYPE && e->typeK->base != stringTYPE) {
			printf("Error: ill-typed print statement (expression does not resolve to int, float64, bool, string or rune).\n");
			exit(1);
		}
	}
}

void checkPrintln(EXP *e, SymbolTable *sym) 
{	
	SYMBOL *s; 

	if (e != NULL) {

		checkPrintln(e->next, sym);

		if (e->kind == k_expressionIdentifier) {

			s = getSymbol(sym, e->val.identifierE.id);

			if (s != NULL) {

				if (s->kind == variableSym) {
					
					if (s->type != NULL && s->type->isArray) {
						printf("Error: cannot print array struct.\n");
						exit(1);
					}

				}

			}

		}	
		
		if (e->kind == k_expressionFunctionCall) {

			s = getSymbol(sym, e->val.funcE.id);

			if (s != NULL) {


				if (s->val.declarationS->val.functionD.returnType == NULL) {
					printf("Error: cannot print function call without return type.\n");
					exit(1);
				}
			}
		}
		
		if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != boolTYPE && e->typeK->base != runeTYPE && e->typeK->base != stringTYPE) {
			printf("Error: ill-typed print statement (expression does not resolve to int, float64, bool, string or rune).\n");
			exit(1);
		}
	}
}

void checkIncrement(EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;

	if (e != NULL) {
		
		if (e->kind != k_expressionArray ) {

			if (e->kind != k_expressionIdentifier) {

				printf("Error: can only incremement/decrement such array or identifier expresssions.\n");
				exit(1);
			}
			else {
				
				s = getSymbol(sym, e->val.identifierE.id);

				if (s != NULL) {

					if (e->isArray) {
						printf("Error: illegal increment/decrement.\n");
						exit(1);
					}
				}
			}
		}
		else {
			printf("Error: can only incremement/decrement such array or identifier expresssions.\n");
			exit(1);
		} 

		if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != runeTYPE) {

			printf("Error: ill-typed increment statement (expression does not resolve to int, float64 or rune).\n");
			exit(1);
		}
	}
}

void checkDecrement(EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;

	if (e != NULL) {
		
		if (e->kind != k_expressionArray ) {

			if (e->kind != k_expressionIdentifier) {

				printf("Error: can only incremement/decrement such array or identifier expresssions.\n");
				exit(1);
			}
			else {
				
				s = getSymbol(sym, e->val.identifierE.id);

				if (s != NULL) {

					if (e->isArray) {
						printf("Error: illegal incremement/decrement.\n");
						exit(1);
					}
				}
			}
		}
		else {
			printf("Error: can only incremement/decrement such array or identifier expresssions.\n");
			exit(1);
		} 

		if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != runeTYPE) {

			printf("Error: ill-typed decrement statement (expression does not resolve to int, float64 or rune).\n");
			exit(1);
		}
	}
}

void checkIfCondition(EXP *e)
{
	if (e != NULL) {

		if (e->typeK->base != boolTYPE) {
			printf("Error: boolean type expected in condition expression.\n");
			exit(1);
		} 
	}
}

void checkSwitch(EXP *e, STATEMENT *clause, SymbolTable *sym)
{		
	SYMBOL *s;
	// cases => expr->superficialType
	if (e != NULL) {

		if (e->kind == k_expressionFunctionCall) {

			s = getSymbol(sym, e->val.funcE.id);

			if (s != NULL) {

				if (s->val.declarationS->val.functionD.returnType == NULL) {
					printf("Error: illegal switch condition (function call with no return type)\n");
					exit(1);
				}
			}
		}

		checkSwitch1(e, clause);
	}
	// cases => boolean
	else {
		checkSwitch2(clause);
	}
}

void checkSwitch1(EXP *e, STATEMENT *clause)
{
	if (clause != NULL) {

		checkSwitch1(e, clause->nextClause);

		if (clause->kind == k_statementSwitchCase) {
			checkCase1(e, clause->val.caseclauseS.explist);
		}
		else if (clause->kind == k_statementSwitchDefault) {
			return;
		}
		else {
			printf("Error: illegal clause statement.\n");
			exit(1);
		}
	}
}

void checkSwitch2(STATEMENT *clause)
{
	if (clause != NULL) {
		
		checkSwitch2(clause->nextClause);

		if (clause->kind == k_statementSwitchCase) {
			checkCase2(clause->val.caseclauseS.explist);
		}
		else if (clause->kind == k_statementSwitchDefault) {
			return;
		}
		else {
			printf("Error: illegal clause statement.\n");
			exit(1);
		}
	}
}

void checkCase1(EXP *expr, EXP *e) 
{
	if (e != NULL) {

		checkCase1(expr, e->next);

		if (expr->typeK != e->typeK) {
			printf("Error: mismatching types in switch case statement(s).\n");
			exit(1);
		}
	}
}

void checkCase2(EXP *e) 
{
	if (e != NULL) {

		checkCase2(e->next);
		
		if (e->typeK != boolTYPE) {
			printf("Error: expected boolean in clause condition.\n");
			exit(1);
		}
	}
}

void checkFor(EXP *e)
{
	if (e != NULL) {

		if (e->typeK->base != boolTYPE) {
			printf("Error: expected boolean in for clause condition.\n");
			exit(1);
		}
	}
}

/* UNARY OPERATIONS */

void checkUplus(EXP *e)
{
	if (e != NULL) {

		if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != runeTYPE) {
			printf("Error: illegal unary plus typing (int, float64 or rune expected).\n");
			exit(1);
		}
	}
}

void checkUminus(EXP *e)
{
	if (e != NULL) {
		
		if (e->typeK->base != intTYPE && e->typeK->base != float64TYPE && e->typeK->base != runeTYPE) {
			printf("Error: illegal unary plus typing (int, float64 or rune expected).\n");
			exit(1);
		}
	}
}

void checkUnot(EXP *e)
{
	if (e != NULL) {

		if (e->typeK->base != boolTYPE) {

			printf("Error: illegal unary negation typing (bool expected).\n");
			exit(1);
		}
	}
}

void checkUxor(EXP *e)
{
	if (e != NULL) {

		if (e->typeK->base != intTYPE && e->typeK->base != runeTYPE) {
			printf("Error: illegal unary bitwise negation typing (int or rune expected).\n");
			exit(1);
		}
	}
}

/* BINARY OPERATIONS */

void checkTor(EXP *par, EXP *left, EXP *right)
{	
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched || binary operation types.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base != boolTYPE && right->typeK->base != boolTYPE) {
			printf("Error: illegal || binary operation typing.\n");
			exit(1);
		}

		par->typeK = left->typeK;	
	}
}

void checkTand(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched && binary operation types.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base != boolTYPE && right->typeK->base != boolTYPE) {
			printf("Error: illegal && binary operation typing.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

void checkEq(EXP *par, EXP *left, EXP *right, SymbolTable *sym)
{	
	SYMBOL *s1, *s2;

	if (par != NULL && left != NULL && right != NULL) {
		
		if (left->typeK != right->typeK) {
			printf("Error: type mismatched == binary operation type arguments.\n");
			exit(1);	
		}

		if (left->kind == k_expressionIdentifier && right->kind == k_expressionIdentifier) {
			
			s1 = getSymbol(sym, left->val.identifierE.id);
			s2 = getSymbol(sym, right->val.identifierE.id);

			if (s1 != NULL && s2 != NULL) {

				if (s1->type->kind == k_typeArray && s2->type->kind == k_typeArray) { 

					compareArrayType(s1->type, s2->type);
				}
			}
		}

		par->typeK = boolTYPE;
	}
}

void checkNeq(EXP *par, EXP *left, EXP *right, SymbolTable *sym)
{
	SYMBOL *s1, *s2;

	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: type mismatched != binary operation type arguments.\n");
			exit(1);	
		}

		if (left->kind == k_expressionIdentifier && right->kind == k_expressionIdentifier) {
			
			s1 = getSymbol(sym, left->val.identifierE.id);
			s2 = getSymbol(sym, right->val.identifierE.id);

			if (s1 != NULL && s2 != NULL) {

				if (s1->type->kind == k_typeArray && s2->type->kind == k_typeArray) { 

					compareArrayType(s1->type, s2->type);
				}
			}
		}

		par->typeK = boolTYPE;
	}
}

void checkL(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {
		
		if (left->typeK != right->typeK) {
			printf("Error: type mismatched < binary operation type arguments.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean arguments in < binary operation.\n");
			exit(1);
		}

		par->typeK = boolTYPE;
	}
}

void checkLeq(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {
		
		if (left->typeK != right->typeK) {
			printf("Error: type mismatched <= binary operation type arguments.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean arguments in <= binary operation.\n");
			exit(1);
		}

		par->typeK = boolTYPE;
	}
}

void checkG(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {
		
		if (left->typeK != right->typeK) {
			printf("Error: type mismatched > binary operation type arguments.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean arguments in > binary operation.\n");
			exit(1);
		}

		par->typeK = boolTYPE;
	}
}

void checkGeq(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {
		
		if (left->typeK != right->typeK) {
			printf("Error: type mismatched >= binary operation type arguments.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean arguments in >= binary operation.\n");
			exit(1);
		}

		par->typeK = boolTYPE;
	}
}

void checkPlus(EXP *par, EXP *left, EXP *right)
{	
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched + binary operation arguments.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean in + binary operation.\n");
			exit(1);
		}

		par->typeK = left->typeK;
	}
}

void checkMinus(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {
		
		if (left->typeK != right->typeK) {
			printf("Error: mismatched - binary operation arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean in - binary operation.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		if (left->typeK->base == stringTYPE || right->typeK->base == stringTYPE) {
			printf("Error: illegal boolean in - binary operation.\n");
			exit(1);
		}

		par->typeK = left->typeK;	
	}
}

void checkMultiply(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {
	
		if (left->typeK != right->typeK) {
			printf("Error: mismatched * binary operation arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean in * binary operation.\n");
			exit(1);
		}

		if (left->typeK->base == stringTYPE || right->typeK->base == stringTYPE) {
			printf("Error: illegal boolean in * binary operation.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		par->typeK = left->typeK;		
	}
}

void checkDivide(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {
		
		if (left->typeK != right->typeK) {
			printf("Error: mismatched / binary operation arguments.\n");
			exit(1);
		}

		if (left->typeK->base == boolTYPE || right->typeK->base == boolTYPE) {
			printf("Error: illegal boolean in / binary operation.\n");
			exit(1);
		}

		if (left->typeK->base == stringTYPE || right->typeK->base == stringTYPE) {
			printf("Error: illegal boolean in / binary operation.\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}

		par->typeK = left->typeK;		
	}
}

void checkOr(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched | binary operation arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base == intTYPE && right->typeK->base == intTYPE) {
			
		}
		else if (left->typeK->base == runeTYPE && right->typeK->base == runeTYPE) {

		}
		else {
			printf("Error: illegal | binary operation typing (integers expected only).\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

void checkXor(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched ^ binary operation arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base == intTYPE && right->typeK->base == intTYPE) {
			
		}
		else if (left->typeK->base == runeTYPE && right->typeK->base == runeTYPE) {

		}
		else {
			printf("Error: illegal ^ binary operation typing (integers expected only).\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

void checkMod(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched %% binary operation arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base == intTYPE && right->typeK->base == intTYPE) {
			
		}
		else if (left->typeK->base == runeTYPE && right->typeK->base == runeTYPE) {

		}
		else {
			printf("Error: illegal %% binary operation typing (integers expected only).\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

void checkLshift(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched << binary operation arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base == intTYPE && right->typeK->base == intTYPE) {
			
		}
		else if (left->typeK->base == runeTYPE && right->typeK->base == runeTYPE) {

		}
		else {
			printf("Error: illegal << binary operation typing (integers expected only).\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

void checkRshift(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched >> binary operation arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base == intTYPE && right->typeK->base == intTYPE) {
			
		}
		else if (left->typeK->base == runeTYPE && right->typeK->base == runeTYPE) {

		}
		else {
			printf("Error: illegal >> binary operation typing (integers expected only).\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

void checkAnd(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched & binary operation arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base == intTYPE && right->typeK->base == intTYPE) {
			
		}
		else if (left->typeK->base == runeTYPE && right->typeK->base == runeTYPE) {

		}
		else {
			printf("Error: illegal & binary operation typing (integers expected only).\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

void checkNand(EXP *par, EXP *left, EXP *right)
{
	if (par != NULL && left != NULL && right != NULL) {

		if (left->typeK != right->typeK) {
			printf("Error: mismatched &^ binary operation arguments.\n");
			exit(1);
		}
		
		if (left->typeK->base == intTYPE && right->typeK->base == intTYPE) {
			
		}
		else if (left->typeK->base == runeTYPE && right->typeK->base == runeTYPE) {

		}
		else {
			printf("Error: illegal &^ binary operation typing (integers expected only).\n");
			exit(1);
		}

		if (left->isArray || right->isArray) {
			printf("Error: no binary operation with array construct arguments.\n");
			exit(1);
		}
		
		par->typeK = left->typeK;		
	}
}

/* ASSIGNMENT LEFT-SIDE HELPER FUNCTION */
void checkAssignmentLeft(EXP *e, SymbolTable *sym)
{		
	SYMBOL *s;

	if (e != NULL) {

		checkAssignmentLeft(e->next, sym);

		switch (e->kind) {

			case k_expressionIdentifier:

				if (strcmp(e->val.identifierE.id, "_") == 0) {
					// ignore
				}
				else {
					s = getSymbol(sym, e->val.identifierE.id);

					if (s != NULL) {

						if (s->kind != variableSym) {
							
							if (s->kind != formalSym) {
								printf("Error: left assignment must refer to a declared variable or a formal.\n");
								exit(1);
							}
						}
					}
				}
				break;
			case k_expressionParenthesis:
				checkAssignmentLeft(e->val.parenthesisE.body, sym);
				break;
			case k_expressionInt:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionFloat64:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionBool:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionString:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionRune:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionUplus:	
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionUminus:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionUnot:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionUxor:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionTor:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionTand:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionEq:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionNeq:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionL:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionLeq:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionG:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionGeq:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionPlus:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionMinus:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionOr:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionXor:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionMultiply:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionDivide:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionMod:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionLshift:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionRshift:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionAnd:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionNand:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionFunctionCall:
				printf("Error: illegal assignment statement construct.\n");
				exit(1);
				break;
			case k_expressionArray:
				checkIndexLeft(e->val.arrayE.exp, sym);
				break;
		}
	}
}

void checkIndexLeft(EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;

	if (e != NULL) {

		if (e->kind == k_expressionIdentifier) {

			s = getSymbol(sym, e->val.identifierE.id);

			if (s != NULL) {

				if (!s->type->isArray) {
					printf("Error: declared variable is not an array.\n");
					exit(1);
				}
			}
		}
		else if (e->kind == k_expressionArray) {
			checkIndexLeft(e->val.arrayE.exp, sym);
		}
		else {
			printf("Error: illegal array construct.\n");
			exit(1);
		}
	}
}

/* FUNCTION EXPRESSION HELPER FUNCTION */
void checkFunctionExpressionFormal(FORMAL *f, EXP *e)
{
	if (f != NULL && e != NULL) {

		checkFunctionExpressionFormal(f->next, e->next);

		if (f->type->typeK != e->typeK) {
			printf("Error: illegal function argument typing.\n");
			exit(1);
		}
	}
	else if (f == NULL && e == NULL) {
		return;
	}
	else {
		printf("Error: function arguments mismatch.\n");
		exit(1);
	}
}

/* ARRAY EXPRESSION HELPER FUNCTIONS */
void iterateArray(DECLARATION *d, EXP *parent, EXP *recursive, TYPEK *k, SymbolTable *sym)
{	
	SYMBOL *s;

	if (parent != NULL && recursive != NULL) {

		switch (recursive->kind) {

			case k_expressionIdentifier:

				s = getSymbol(sym, recursive->val.identifierE.id);
				
				if (s != NULL) {
					
					parent->typeK = s->typeK;
				}
				else {
					printf("Error: array undefined.\n");
					exit(1);
				}

				break;
			case k_expressionParenthesis:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionInt:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionFloat64:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionBool:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionString:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionRune:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionUplus:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionUminus:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionUnot:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionUxor:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionTor:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionTand:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionEq:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionNeq: 
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionL:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionLeq:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionG:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionGeq:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionPlus:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionMinus:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionOr:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionXor:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionMultiply:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionDivide:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionMod:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionLshift:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionRshift:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionAnd:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionNand:
				printf("Error: illegal array construct.\n");
				exit(1);	
				break;
			case k_expressionFunctionCall:
				printf("Error: illegal array construct.\n");
				exit(1);
				break;
			case k_expressionArray:
				symImplementationEXP(d, recursive->val.arrayE.value, sym);
				if (recursive->val.arrayE.value->typeK->base != intTYPE) {
					printf("Error: illegal array construct.\n");
					exit(1);
				}
				iterateArray(d, parent, recursive->val.arrayE.exp, k, sym);

				break;
		}
	}
}

void checkArray(DECLARATION *d, EXP *e, SymbolTable *sym) {

	if (e != NULL) {

		iterateArray(d, e, e->val.arrayE.exp, e->val.arrayE.value->typeK, sym);
	}
}

/* VARIABLE DECLARATION CASE 2 HELPER FUNCTION */
void setVariableType3(EXP *e, IDL *id)
{
	if (e != NULL && id != NULL) {

		setVariableType3(e, id->next);

		if (strcmp(e->val.identifierE.id, id->name) == 0) {
			e->typeK = id->typeK;
		}
	}
}

/* SPECIAL FUNCTIONS TYPE CHECK */
void checkSpecialFunctions(DECLARATION *d)
{
	if (d != NULL) {

		if (strcmp(d->val.functionD.id, "init") == 0) {

			checkInvalidFormal(d->val.functionD.arguments);
			checkInvalidReturn(d->val.functionD.returnType);
		}

		if (strcmp(d->val.functionD.id, "main") == 0) {
			
			checkInvalidFormal(d->val.functionD.arguments);
			checkInvalidReturn(d->val.functionD.returnType);
		}
	}
}

void checkInvalidFormal(FORMAL *f)
{
	if (f != NULL) {
		printf("Error: illegal formal(s) in special function.\n");
		exit(1);
	}
}

void checkInvalidReturn(TYPE *t)
{
	if (t != NULL) {
		printf("Error: illegal return type in special function.\n");
		exit(1);
	}
}

void checkSpecialFunctionScope(EXP *e)
{
	if (e != NULL) {

		if (strcmp(e->val.funcE.id, "init") == 0) {
			printf("Error: illegal init function in scope.\n");
			exit(1);
		}

		if (strcmp(e->val.funcE.id, "main") == 0) {
			printf("Error: illegal main function in scope.\n");
			exit(1);
		}
	}
}

void filterRecursive(IDL *id, TYPE *t, SymbolTable *sym)
{	
	SYMBOL *s;

	if (id != NULL && t != NULL) {

		filterRecursive(id->next, t, sym);

		switch (t->kind) {

			case k_typeReference:
				s = getSymbol(sym, t->val.refT.id);
		
				if (s != NULL) {

					

					if (strcmp(s->name, id->name) == 0) {
						printf("Error: type undefined in previous declarations.\n");
						exit(1);
					}
				}
				break;
			case k_typeArray: 
				filterRecursive(id, t->val.arrayT.type, sym);
				break;
			case k_typeParenthesis:
				filterRecursive(id, t->val.parT.type, sym);
				break;
		}
	}
}

void filterInit(IDL *id)
{
	if (id != NULL) {

		if (strcmp(id->name, "init") == 0) {
			printf("Error: illegal variable/type declaration with 'init' identifier.\n");
			exit(1);
		}
	}
}

void filterExpressionStatement(EXP *e, SymbolTable *sym)
{	
	SYMBOL *s;

	if (e != NULL) {

		if (e->kind == k_expressionFunctionCall) {

			s = getSymbol(sym, e->val.funcE.id);

			if (s != NULL) {

				if (s->kind != functionSym) {
					printf("Error: with the exception of specific built-in functions, only function and method calls and receive operations can appear in expression statements.\n");
					exit(1);
				}
			}
		}
		else {
			printf("Error: with the exception of specific built-in functions, only function and method calls and receive operations can appear in expression statements.\n");
			exit(1);
		}
	}
}

void checkPreDeclaration(int parent, int child)
{	
	if (child > parent) {

		printf("Error: illegal function call. The function is not defined prior to the parent function.\n");
		exit(1);
	}
}

int countCast(EXP *e) 
{
	if (e != NULL) {
		return 1 + countCast(e->next);
	}
	else {
		return 0;
	}
}








