#include <stdio.h>
#include "pretty.h"

/* NOTE: 
 * Pretty print program
 */
 
void prettyPROGRAM(PROGRAM *p) 
{
	if (p != NULL) {
		prettyPACKAGE(p->package);
		printf("\n");
		prettyDECLARATION(p->declarations);
	}
}

// Pretty print package
void prettyPACKAGE(PACKAGE *p)
{
	if (p != NULL) {
		printf("package %s\n", p->name);
	}
}

// Pretty print declaration
void prettyDECLARATION(DECLARATION *d)
{
	if (d != NULL) {

		switch (d->kind) {
			// Variable declaration 
			case k_declarationVariable:
				// Type only
				if (d->val.variableD.format == 1) {
					printf("var ");
					prettyIDL(d->val.variableD.ids);
					printf(" ");
					prettyTYPE(d->val.variableD.type);
				}
				// Expression only
				else if (d->val.variableD.format == 2) {
					printf("var ");
					prettyIDL(d->val.variableD.ids);
					printf(" = ");
					prettyEXP(d->val.variableD.expls);
				}
				// Type and expression
				else if (d->val.variableD.format == 3) {
					printf("var ");
					prettyIDL(d->val.variableD.ids);
					printf(" ");
					prettyTYPE(d->val.variableD.type);
					printf(" = ");
					prettyEXP(d->val.variableD.expls);
				}
				break;
			// Type declaration
			case k_declarationType:
				printf("type ");
				prettyIDL(d->val.typeD.ids);
				printf(" ");
				prettyTYPE(d->val.typeD.type);
				printf("\n");
				break;
			// Function declaration
			case k_declarationFunction:
				printf("func ");
				printf("%s(", d->val.functionD.id);
				prettyFORMAL(d->val.functionD.arguments);
				printf(") ");
				if (d->val.functionD.returnType != NULL) {
					prettyTYPE(d->val.functionD.returnType);
					printf(" ");
				}
				printf("{\n");
				prettySTATEMENT(d->val.functionD.body, 0, 1);
				printf("\n}\n");
				break;
		}
		printf("\n");
		// Next declaration
		prettyDECLARATION(d->next);
	}

}

// Pretty print identifier list
void prettyIDL(IDL *i) 
{
	if (i != NULL) {
		// Appriorate comma for identifier list
		if (i->next != NULL) {
			prettyIDL(i->next);
			printf(", ");
		}
		else {
			prettyIDL(i->next);
		}
		printf("%s", i->name);
	}
}

// Pretty print type
void prettyTYPE(TYPE *t) 
{
	if (t != NULL) {

		switch (t->kind) {
			// Identifier type (i.e. int, float64, personalized, etc)
			case k_typeReference:
				printf("%s", t->val.refT.id);
				break;
			// Array type
			case k_typeArray:
				printf("[");
				printf("%d", t->val.arrayT.value);
				printf("]");
				prettyTYPE(t->val.arrayT.type);
				break;
			// Parenthesized type
			case k_typeParenthesis:
				printf("(");
				prettyTYPE(t->val.parT.type);
				printf(")");
				break;
		}
	}
}

// Pretty print formal
void prettyFORMAL(FORMAL *f) 
{
	if (f != NULL) {
		// Appropriate comma
		if (f->next != NULL) {
			prettyFORMAL(f->next);
			printf(", ");
		}
		else {
			prettyFORMAL(f->next);
		}
		printf("%s ", f->name);
		// Next formal
		prettyTYPE(f->type);
	}
}

// Pretty print statement
void prettySTATEMENT(STATEMENT *s, int extraTab, int nLine) 
{
	if (s != NULL) {

		// Number of appriopriate tabs for nested statements
		if (extraTab == 0) {
			printf("\t");
		}
		if (extraTab == 1) {
			printf("\t\t");
		}
		if (extraTab == -1) {
			printf(" ");
		}

		switch (s->kind) {
			// Block
			case k_statementBlock:
				printf("{\n");
				prettySTATEMENT(s->val.blockS.body, 1, nLine);
				printf("\t}");
				break;
			// Expression statement
			case k_statementExpression:
				prettyEXP(s->val.expressionS.exp);
				break;
			// Assignment expression
			case k_statementAssignment:
				prettyEXP(s->val.assignmentS.ids);
				printf(" = ");
				prettyEXP(s->val.assignmentS.exp);
				break;
			// Declaration statement
			case k_statementDeclaration:
				prettyDECLARATION(s->val.declarationS.decl);
				break;
			// Increment statement
			case k_statementIncrement:
				prettyEXP(s->val.incrementS.exp);
				printf("++");
				break;
			// Decrement statement
			case k_statementDecrement:
				prettyEXP(s->val.decrementS.exp);
				printf("--");
				break;
			// Print statement
			case k_statementPrint:
				printf("print(");
				prettyEXP(s->val.printS.printlist);
				printf(")");
				break;
			// Println statement
			case k_statementPrintln:
				printf("println(");
				prettyEXP(s->val.printlnS.printlist);
				printf(")");
				break;
			// Return statement
			case k_statementReturn:
				printf("return ");
				prettyEXP(s->val.returnS.exp);
				break;
			// If statement
			case k_statementIf:
				printf("if ");
				prettyEXP(s->val.ifS.condition);
				printf(" {\n");
				prettySTATEMENT(s->val.ifS.body, 1, nLine);
				printf("\t}");
				break;
			// If else statement
			case k_statementIfElse:
				printf("if ");
				prettyEXP(s->val.ifelseS.condition);
				printf(" {\n\t");
				prettySTATEMENT(s->val.ifelseS.thenPart, 1, nLine);
				printf("\t}");
				printf(" else");
				// else if statements in between
				if (s->nextIf != NULL) {
					prettySTATEMENT(s->nextIf, -1, nLine);
				}
				else {
					printf(" {\n");
					prettySTATEMENT(s->val.ifelseS.elsePart, 1, nLine);
					printf("\t}");
				}
				break;
			// Switch statment
			case k_statementSwitch:
				printf("switch (");
				prettyEXP(s->val.switchS.condition);
				printf(") {\n\t");
				prettySTATEMENT(s->val.switchS.clauses, 1, nLine);
				printf("\t}");
				break;
			// Case statement
			case k_statementSwitchCase:
				printf("case ");
				prettyEXP(s->val.caseclauseS.explist);
				printf(": ");
				prettySTATEMENT(s->val.caseclauseS.content, -1, nLine);
				prettySTATEMENT(s->nextClause, extraTab, nLine);
				break;
			// Default statement
			case k_statementSwitchDefault:
				printf("default: ");
				prettySTATEMENT(s->val.defaultclauseS.content, -1, nLine);
				prettySTATEMENT(s->nextClause, extraTab, nLine);
				break;
			// For statement
			case k_statementFor:
				printf("for ");
				
				// Output appriopriate format
				if (s->val.forS.infinitel) {
					printf("{\n");
					prettySTATEMENT(s->val.forS.body, 1, nLine);
					printf("\t}");
				}
				else if (s->val.forS.whilel) {
					prettyEXP(s->val.forS.condition);
					printf(" {\n");
					prettySTATEMENT(s->val.forS.body, 1, nLine);
					printf("\t}");
				}
				else if (s->val.forS.threel) {

					prettySTATEMENT(s->val.forS.initStmt, -1, -1);
					printf("; ");
					prettyEXP(s->val.forS.condition);
					printf("; ");
					prettySTATEMENT(s->val.forS.postStmt, -1, -1);
					printf(" {\n");
					prettySTATEMENT(s->val.forS.body, 1, nLine);
					printf("\t}");
				}
				break;
			// Break statement
			case k_statementBreak:
				printf("break");
				break;
			// Continue statement
			case k_statementContinue:
				printf("continue");
				break;
			// Skip statement
			case k_statementSkip:
				printf(";");
				break;
		}
		// New line if approrpriate
		if (nLine == 1) {
			printf("\n");
		}
		prettySTATEMENT(s->next, extraTab, nLine);
	}
}

// Pretty print expression
void prettyEXP(EXP *e) 
{	
	if (e != NULL) {
		// Comma for expression lists
		if (e->next != NULL) {
			prettyEXP(e->next);
			printf(", ");
		}
		else {
			prettyEXP(e->next);
		}

		switch (e->kind) {

			case k_expressionIdentifier:
				printf("%s", e->val.identifierE.id);
				break; 
			case k_expressionParenthesis:
				printf("(");
				prettyEXP(e->val.parenthesisE.body);
				printf(")");
				break; 
			case k_expressionInt:
				printf("%d", e->val.intconst);
				break; 
			case k_expressionFloat64:
				printf("%s", e->val.floatconst);
				break; 
			case k_expressionBool:
				if (e->val.boolconst == 1) {
					printf("true");
				}
				else if (e->val.boolconst == 0) {
					printf("false");
				}
				break; 
			case k_expressionString:
				printf("%s", e->val.stringconst);
				break; 
			case k_expressionRune:
				printf("%s", e->val.runeconst);
				break; 
			case k_expressionUplus:
				printf("+");
				prettyEXP(e->val.uplusE.body);
				break; 
			case k_expressionUminus:
				printf("-");
				prettyEXP(e->val.uminusE.body);
				break; 
			case k_expressionUnot:
				printf("!");
				prettyEXP(e->val.unotE.body);
				break; 
			case k_expressionUxor:
				printf("^");
				prettyEXP(e->val.uxorE.body);
				break; 
			case k_expressionTor:
				prettyEXP(e->val.torE.left);
				printf(" || ");
				prettyEXP(e->val.torE.right);
				break; 
			case k_expressionTand:
				prettyEXP(e->val.tandE.left);
				printf(" && ");
				prettyEXP(e->val.tandE.right);	
				break; 
			case k_expressionEq:
				prettyEXP(e->val.eqE.left);
				printf(" == ");
				prettyEXP(e->val.eqE.right);
				break; 
			case k_expressionNeq: 
				prettyEXP(e->val.neqE.left);
				printf(" != ");
				prettyEXP(e->val.neqE.right);
				break; 
			case k_expressionL:
				prettyEXP(e->val.lE.left);
				printf(" < ");
				prettyEXP(e->val.lE.right);
				break; 
			case k_expressionLeq:
				prettyEXP(e->val.leqE.left);
				printf(" <= ");
				prettyEXP(e->val.leqE.right);
				break; 
			case k_expressionG:
				prettyEXP(e->val.gE.left);
				printf(" > ");
				prettyEXP(e->val.gE.right);
				break; 
			case k_expressionGeq:
				prettyEXP(e->val.geqE.left);
				printf(" >= ");
				prettyEXP(e->val.geqE.right);
				break; 
			case k_expressionPlus:
				prettyEXP(e->val.plusE.left);
				printf(" + ");
				prettyEXP(e->val.plusE.right);
				break; 
			case k_expressionMinus:
				prettyEXP(e->val.minusE.left);
				printf(" - ");
				prettyEXP(e->val.minusE.right);
				break; 
			case k_expressionOr:
				prettyEXP(e->val.plusE.left);
				printf(" | ");
				prettyEXP(e->val.plusE.right);
				break; 
			case k_expressionXor:
				prettyEXP(e->val.xorE.left);
				printf(" ^ ");
				prettyEXP(e->val.xorE.right);
				break; 
			case k_expressionMultiply:
				prettyEXP(e->val.multiplyE.left);
				printf(" * ");
				prettyEXP(e->val.multiplyE.right);
				break; 
			case k_expressionDivide:
				prettyEXP(e->val.divideE.left);
				printf(" / ");
				prettyEXP(e->val.divideE.right);
				break; 
			case k_expressionMod:
				prettyEXP(e->val.modE.left);
				printf(" %% ");
				prettyEXP(e->val.modE.right);
				break; 
			case k_expressionLshift:
				prettyEXP(e->val.lshiftE.left);
				printf(" <<");
				prettyEXP(e->val.lshiftE.right);
				break; 
			case k_expressionRshift:
				prettyEXP(e->val.rshiftE.left);
				printf(" >> ");
				prettyEXP(e->val.rshiftE.right);
				break; 
			case k_expressionAnd:
				prettyEXP(e->val.andE.left);
				printf(" & ");
				prettyEXP(e->val.andE.right);
				break; 
			case k_expressionNand:
				prettyEXP(e->val.nandE.left);
				printf(" &^ ");
				prettyEXP(e->val.nandE.right);
				break; 
			case k_expressionFunctionCall:
				printf("%s(", e->val.funcE.id);
				prettyEXP(e->val.funcE.explist);
				printf(")");
				break; 
			case k_expressionArray: 
				prettyEXP(e->val.arrayE.exp);
				printf("[");
				prettyEXP(e->val.arrayE.value);
				printf("]");
				break;
		}
	}
}












