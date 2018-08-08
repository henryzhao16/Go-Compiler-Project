#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "weed.h"

/* NOTE:
 * Function to weed the program
 */
void weedPROGRAM(PROGRAM *p)
{
	if (p != NULL) {
		weedDECLARATION(p->declarations);
	}
}

// Function to weed declaration
void weedDECLARATION(DECLARATION *d)
{
	if (d != NULL) {

		switch (d->kind) {

			case k_declarationVariable:
				weedDECLARATION_variable(d->val.variableD.ids, d->val.variableD.expls, d->val.variableD.expressionAssigned);
				break;
			case k_declarationType:
				break;
			case k_declarationFunction:
				weedSTATEMENTbreak(d->val.functionD.body,0);
				weedSTATEMENTcontinue(d->val.functionD.body,0);
				weedSTATEMENTdefault(d->val.functionD.body);
				weedSTATEMENTassignment(d->val.functionD.body);
				weedSTATEMENTexpression(d->val.functionD.body);

				if (d->val.functionD.returnType == NULL) {
					weedSTATEMENTreturn(d->val.functionD.body, 0);
				}
				else {
					weedSTATEMENTreturn(d->val.functionD.body, 1);
				}

				weedSTATEMENTforclause(d->val.functionD.body);
				weedSTATEMENTforclause2(d->val.functionD.body);

				break;
		}
		weedDECLARATION(d->next);
	}
}

// Function to weed variable declaration 
void weedDECLARATION_variable(IDL *id, EXP *e, int expressionAssigned) 
{
	if (expressionAssigned) {
		
		int lhs = checkLHS_variable(id, 0);
		int rhs = checkRHS_variable(e, 0);

		if (lhs != rhs) {
			printf("Error: The LHS variable declaration is of size %d and the RHS variable declaration is of size %d.\n", lhs, rhs);
			exit(1);
		}
	}
}

// Function to weed break statement
void weedSTATEMENTbreak(STATEMENT *s, int breakAllowed) 
{
	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:
				weedSTATEMENTbreak(s->val.blockS.body,breakAllowed);
				break;
			case k_statementExpression:
				break;
			case k_statementAssignment:
				break;
			case k_statementDeclaration:
				break;
			case k_statementIncrement:
				break;
			case k_statementDecrement:
				break;
			case k_statementPrint:
				break;
			case k_statementPrintln:
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				weedSTATEMENTbreak(s->val.ifS.body,breakAllowed);
				break;
			case k_statementIfElse:
				weedSTATEMENTbreak(s->val.ifelseS.thenPart,breakAllowed);
				weedSTATEMENTbreak(s->val.ifelseS.elsePart,breakAllowed);
				break;
			case k_statementSwitch:
				weedSTATEMENTbreak(s->val.switchS.clauses,breakAllowed);
				break;
			case k_statementSwitchCase:
				weedSTATEMENTbreak(s->nextClause,1);
				break;
			case k_statementSwitchDefault:
				weedSTATEMENTbreak(s->nextClause,1);
				break;
			case k_statementFor:
				weedSTATEMENTbreak(s->val.forS.initStmt,breakAllowed);
				weedSTATEMENTbreak(s->val.forS.postStmt,breakAllowed);
				weedSTATEMENTbreak(s->val.forS.body,1);
				break;
			case k_statementBreak:
				if (!breakAllowed) {
					printf("Error: The break statement is only allowed in for and switch case/defautl statements.\n");
					exit(1);
				}
				break;
			case k_statementContinue:
				break;
			case k_statementSkip:
				break;
		}
		weedSTATEMENTbreak(s->next, breakAllowed);
	}
}

// Function to weed continue statement
void weedSTATEMENTcontinue(STATEMENT *s, int continueAllowed) 
{
	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:
				weedSTATEMENTcontinue(s->val.blockS.body,continueAllowed);
				break;
			case k_statementExpression:
				break;
			case k_statementAssignment:
				break;
			case k_statementDeclaration:
				break;
			case k_statementIncrement:
				break;
			case k_statementDecrement:
				break;
			case k_statementPrint:
				break;
			case k_statementPrintln:
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				weedSTATEMENTcontinue(s->val.ifS.body,continueAllowed);
				break;
			case k_statementIfElse:
				weedSTATEMENTcontinue(s->val.ifelseS.thenPart,continueAllowed);
				weedSTATEMENTcontinue(s->val.ifelseS.elsePart,continueAllowed);
				break;
			case k_statementSwitch:
				weedSTATEMENTcontinue(s->val.switchS.clauses,continueAllowed);
				break;
			case k_statementSwitchCase:
				weedSTATEMENTcontinue(s->nextClause,continueAllowed);
				break;
			case k_statementSwitchDefault:
				weedSTATEMENTcontinue(s->val.defaultclauseS.content,continueAllowed);
				weedSTATEMENTcontinue(s->nextClause,continueAllowed);
				break;
			case k_statementFor:
				weedSTATEMENTcontinue(s->val.forS.initStmt,continueAllowed);
				weedSTATEMENTcontinue(s->val.forS.postStmt,continueAllowed);
				weedSTATEMENTcontinue(s->val.forS.body,1);
				break;
			case k_statementBreak:
				break;
			case k_statementContinue:
				if (!continueAllowed) {
					printf("Error: The continue statement is only allowed inside a for statement.\n");
					exit(1);
				}
				break;
			case k_statementSkip:
				break;
		}
		weedSTATEMENTcontinue(s->next, continueAllowed);
	}
}

// Function to weed default statement
void weedSTATEMENTdefault(STATEMENT *s) 
{
	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:
				weedSTATEMENTdefault(s->val.blockS.body);
				break;
			case k_statementExpression:
				break;
			case k_statementAssignment:
				break;
			case k_statementDeclaration:
				break;
			case k_statementIncrement:
				break;
			case k_statementDecrement:
				break;
			case k_statementPrint:
				break;
			case k_statementPrintln:
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				weedSTATEMENTdefault(s->val.ifS.body);
				break;
			case k_statementIfElse:
				weedSTATEMENTdefault(s->val.ifelseS.thenPart);
				weedSTATEMENTdefault(s->val.ifelseS.elsePart);
				break;
			case k_statementSwitch:
				checkSTATEMENTdefault(s->val.switchS.clauses);
				break;
			case k_statementSwitchCase:
				break;
			case k_statementSwitchDefault:
				break;
			case k_statementFor:
				weedSTATEMENTdefault(s->val.forS.initStmt);
				weedSTATEMENTdefault(s->val.forS.postStmt);
				weedSTATEMENTdefault(s->val.forS.body);
				break;
			case k_statementBreak:
				break;
			case k_statementContinue:
				break;
			case k_statementSkip:
				break;
		}

		weedSTATEMENTdefault(s->next);
	}
}

// Function to weed expression statement 
void weedSTATEMENTexpression(STATEMENT *s)
{
	if (s != NULL) {

		weedSTATEMENTexpression(s->next);

		switch (s->kind) {
			case k_statementBlock:
				break;
			case k_statementExpression:
				checkExpressionStatement(s->val.expressionS.exp);
				break;
			case k_statementAssignment:
				break;
			case k_statementDeclaration:
				break;
			case k_statementIncrement:
				break;
			case k_statementDecrement:
				break;
			case k_statementPrint:
				break;
			case k_statementPrintln:
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				break;
			case k_statementIfElse:
				break;
			case k_statementSwitch:
				break;
			case k_statementSwitchCase:
				break;
			case k_statementSwitchDefault:
				break;
			case k_statementFor:
				break;
			case k_statementBreak:
				break;
			case k_statementContinue:
				break;
			case k_statementSkip:
				break;
		}
	}
}

// Function to weed assignment statement
void weedSTATEMENTassignment(STATEMENT *s) 
{

	if (s != NULL) {

		weedSTATEMENTassignment(s->next);

		switch (s->kind) {

			case k_statementBlock:
				break;
			case k_statementExpression:
				break;
			case k_statementAssignment:
				checkAssignment(s);
				break;
			case k_statementDeclaration:
				break;
			case k_statementIncrement:
				break;
			case k_statementDecrement:
				break;
			case k_statementPrint:
				break;
			case k_statementPrintln:
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				break;
			case k_statementIfElse:
				break;
			case k_statementSwitch:
				break;
			case k_statementSwitchCase:
				break;
			case k_statementSwitchDefault:
				break;
			case k_statementFor:
				break;
			case k_statementBreak:
				break;
			case k_statementContinue:
				break;
			case k_statementSkip:
				break;
		}
	}
}

// Function to weed return statement
void weedSTATEMENTreturn(STATEMENT *s, int returnPresent) 
{	
	if (s != NULL) {

		if (s->next == NULL) {

			switch (s->kind) {

				case k_statementBlock:

					weedSTATEMENTreturn(s->val.blockS.body, returnPresent);
					break;
				case k_statementExpression:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementAssignment:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementDeclaration:

					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementIncrement:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementDecrement:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementPrint:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementPrintln:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementReturn:

					if (returnPresent) {

						if (s->val.returnS.exp == NULL) {
							printf("Error: return statement with expression expected (at the end).\n");
							exit(1);
						}
					}
					else {

						if (s->val.returnS.exp != NULL) {
							printf("Error: return statement with expression unexpected.\n");
							exit(1);
						}
					}
					break;
				case k_statementIf:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					//checkThen(s->val.ifS.body, returnPresent);
					break;
				case k_statementIfElse:
					
					checkThen(s->val.ifelseS.thenPart, returnPresent);
			
					if (s->nextIf != NULL) {
						weedSTATEMENTreturn(s->nextIf, returnPresent);
					}
					else {
						checkThen(s->val.ifelseS.elsePart, returnPresent);
					}
					break;
				case k_statementSwitch:
					checkSwitchSp(s->val.switchS.clauses, returnPresent);
					if (returnPresent) checkSTATEMENTdefault2(s->val.switchS.clauses);
					break;
				case k_statementSwitchCase:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementSwitchDefault:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementFor:
					if (returnPresent) {

						if (s->val.forS.infinitel) {

							checkWithinReturn(s->val.forS.body);
						} 
						else if (s->val.forS.whilel) {
							printf("Error: return statement expected (at the end).\n");
							exit(1);
						} 
						else if (s->val.forS.threel) {

							if (s->val.forS.initStmt == NULL && s->val.forS.condition == NULL && s->val.forS.postStmt == NULL) {

								checkWithinReturn(s->val.forS.body);
							}
							else {
								printf("Error: return statement expected (at the end).\n");
								exit(1);
							}
						}
					}
					break;
				case k_statementBreak:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				case k_statementContinue:
					if (returnPresent) {
						printf("Error: return statement expected.\n");
						exit(1);
					}
					break;
				case k_statementSkip:
					if (returnPresent) {
						printf("Error: return statement expected (at the end).\n");
						exit(1);
					}
					break;
				}	
			}
		else {
			weedSTATEMENTreturn(s->next, returnPresent);
		}
	}
}

// Functions to weed for clause
void weedSTATEMENTforclause(STATEMENT *s)
{
	if (s != NULL) {

		weedSTATEMENTassignment(s->next);

		switch (s->kind) {

			case k_statementBlock:
				break;
			case k_statementExpression:
				break;
			case k_statementAssignment:
				break;
			case k_statementDeclaration:
				break;
			case k_statementIncrement:
				break;
			case k_statementDecrement:
				break;
			case k_statementPrint:
				break;
			case k_statementPrintln:
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				break;
			case k_statementIfElse:
				break;
			case k_statementSwitch:
				break;
			case k_statementSwitchCase:
				break;
			case k_statementSwitchDefault:
				break;
			case k_statementFor:
				if (s->val.forS.threel) {
					if (s->val.forS.initStmt == NULL && s->val.forS.condition != NULL && s->val.forS.postStmt != NULL) {
						printf("Error: illegal for clause.\n");
						exit(1);
					}
				}
				break;
			case k_statementBreak:
				break;
			case k_statementContinue:
				break;
			case k_statementSkip:
				break;
		}
	}
}

void weedSTATEMENTforclause2(STATEMENT *s)
{
	if (s != NULL) {

		weedSTATEMENTassignment(s->next);

		switch (s->kind) {

			case k_statementBlock:
				break;
			case k_statementExpression:
				break;
			case k_statementAssignment:
				break;
			case k_statementDeclaration:
				break;
			case k_statementIncrement:
				break;
			case k_statementDecrement:
				break;
			case k_statementPrint:
				break;
			case k_statementPrintln:
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				break;
			case k_statementIfElse:
				break;
			case k_statementSwitch:
				break;
			case k_statementSwitchCase:
				break;
			case k_statementSwitchDefault:
				break;
			case k_statementFor:
				checkClauseStatement(s->val.forS.initStmt);
				checkClauseStatement(s->val.forS.postStmt);
				break;
			case k_statementBreak:
				break;
			case k_statementContinue:
				break;
			case k_statementSkip:
				break;
		}
	}
}

/* NOTE:
 * The following are helper functions to assist weeding. 
 */

int checkLHS_variable(IDL *i, int lhs)
{
	if (i == NULL) {
		return lhs;
	}
	return checkLHS_variable(i->next, lhs + 1);
}

int checkRHS_variable(EXP *e, int rhs)
{
	if (e == NULL) {
		return rhs;
	}
	return checkRHS_variable(e->next, rhs + 1);
}

int calculateDefault(STATEMENT *s, int n)
{
	int sum = 0;
	sum = sum + n;

	if (s == NULL) {
		return sum;
	}

	if (s->kind == k_statementSwitchDefault) {
		sum++;
	}

	return calculateDefault(s->nextClause, sum);
}

void checkSTATEMENTdefault(STATEMENT *s)
{
	if (s != NULL) {

		int n = 0;

		if (s->kind == k_statementSwitchDefault) {
			n++;
		}

		int total = calculateDefault(s->nextClause, n);

		if (total > 1) {
			printf("Error: There are %d default statements. Only 1 is expected inside a switch statement.\n", total);
			exit(1);
		}
	}
}

void checkSTATEMENTdefault2(STATEMENT *s)
{
	if (s != NULL) {

		int n = 0;

		if (s->kind == k_statementSwitchDefault) {
			n++;
		}

		int total = calculateDefault(s->nextClause, n);

		if (total != 1) {
			printf("Error: There are %d default statements. Only 1 is expected inside a switch statement.\n", total);
			exit(1);
		}
	}
}

void checkExpressionStatement(EXP *e)
{
	if (e != NULL) {

		if (e->kind != k_expressionFunctionCall) {
			printf("Error: The expression statement is assigned incorrectly. Function call is expected.\n");
			exit(1);
		}
	}
}

int checkLHS_assignment(EXP *i, int lhs)
{
	if (i == NULL) {
		return lhs;
	}
	return checkLHS_assignment(i->next, lhs + 1);
}

int checkRHS_assignment(EXP *e, int rhs)
{
	if (e == NULL) {
		return rhs;
	}
	return checkRHS_assignment(e->next, rhs + 1);
}

void checkAssignment(STATEMENT *s) {
	int lhs = checkLHS_assignment(s->val.assignmentS.ids, 0);
	int rhs = checkRHS_assignment(s->val.assignmentS.exp, 0);
	if (lhs != rhs) {
		printf("Error: The LHS assignment is of size %d and the RHS assignment is of size %d.\n", lhs, rhs);
		exit(1);
	}
}

void checkSwitchSp(STATEMENT *s, int returnPresent)
{	
	if (s != NULL) {

		checkSwitchSp(s->nextClause, returnPresent);
		
		if (s->kind == k_statementSwitchCase) {
			checkCaseSp(s->val.caseclauseS.content, returnPresent);
		}
		else if (s->kind == k_statementSwitchDefault) {
			checkCaseSp(s->val.defaultclauseS.content, returnPresent);
		}
	}
}

void checkCaseSp(STATEMENT *s, int returnPresent) {

	if (s != NULL) {

		if (s->next == NULL) {
				
			if (returnPresent) {

				if (s->kind != k_statementReturn) {
					printf("Error: return statement expected (at the end).\n");
					exit(1);
				}
			}
		}
		else {
			checkCaseSp(s->next, returnPresent);
		}
	}
	else {
		if (returnPresent) {
			printf("Error: return statement expected (at the end).\n");
			exit(1);
		}
	}

}

void checkThen(STATEMENT *s, int returnPresent)
{
	if (s != NULL) {

		if (s->next == NULL) {
			
			if (returnPresent) {

				if (s->kind != k_statementReturn) {
					printf("Error: return statement expected (at the end).\n");
					exit(1);
				}
			}
		}
		else {
			checkThen(s->next, returnPresent);
		}
	}
	else {
		if (returnPresent) {
			printf("Error: return statement expected (at the end).\n");
			exit(1);
		}
	}
}

void checkWithinReturn(STATEMENT *s) 
{
	if (s != NULL) {

		checkWithinReturn(s->next);

		switch (s->kind) {

				case k_statementBlock:
					checkWithinReturn(s->val.blockS.body);
					break;
				case k_statementExpression:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementAssignment:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementDeclaration:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementIncrement:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementDecrement:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementPrint:
					break;
				case k_statementPrintln:
					break;
				case k_statementReturn:
					break;
				case k_statementIf:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementIfElse:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementSwitch:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementSwitchCase:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementSwitchDefault:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementFor:
					
					checkWithinReturn(s->val.forS.body);
					break;
				case k_statementBreak:
					printf("Error: return statement expected (at the end).\n");
					exit(1);
					break;
				case k_statementContinue:
					printf("Error: return statement expected.\n");
					exit(1);
					break;
				case k_statementSkip:
					break;
				}			
	}
}

void checkClauseStatement(STATEMENT *s)
{
	if (s != NULL) {

		switch (s->kind) {

			case k_statementBlock:
				break;
			case k_statementExpression:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementAssignment:
				break;
			case k_statementDeclaration:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementIncrement:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementDecrement:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementPrint:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementPrintln:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementReturn:
				break;
			case k_statementIf:
				break;
			case k_statementIfElse:
				break;
			case k_statementSwitch:
				break;
			case k_statementSwitchCase:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementSwitchDefault:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementFor:
				break;
			case k_statementBreak:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementContinue:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
			case k_statementSkip:
				printf("Error: illegal for clause.\n");
				exit(1);
				break;
		}
	}
}
