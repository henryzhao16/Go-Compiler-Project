#include "tree.h"

/* NOTE:
 * The following includes the declarations of weeding functions, along
 * with helper functions.
 */ 

void weedPROGRAM(PROGRAM *p);
void weedDECLARATION(DECLARATION *d);
void weedSTATEMENTbreak(STATEMENT *s, int breakAllowed);
void weedSTATEMENTcontinue(STATEMENT *s, int continueAllowed);
void weedSTATEMENTdefault(STATEMENT *s);
void weedDECLARATION_variable(IDL *id, EXP *e, int expressionAssigned);
void weedSTATEMENTreturn(STATEMENT *s, int returnPresent);
void weedSTATEMENTforclause(STATEMENT *s);
void weedSTATEMENTforclause2(STATEMENT *s);
void weedSTATEMENTassignment(STATEMENT *s);
void weedSTATEMENTexpression(STATEMENT *s);
void checkExpressionStatement(EXP *e);
void checkAssignment(STATEMENT *s);
void checkSTATEMENTdefault(STATEMENT *s);
void checkClauseStatement(STATEMENT *s);
void checkWithinReturn(STATEMENT *s);
void checkThen(STATEMENT *s, int returnPresent);
void checkSwitchSp(STATEMENT *s, int returnPresent);
void checkCaseSp(STATEMENT *s, int returnPresent);
void checkSTATEMENTdefault2(STATEMENT *s);
int checkLHS_variable(IDL *i, int lhs);
int checkRHS_variable(EXP *e, int rhs);
int checkLHS_assignment(EXP *i, int lhs);
int checkRHS_assignment(EXP *e, int rhs);
int calculateDefault(STATEMENT *s, int n);
