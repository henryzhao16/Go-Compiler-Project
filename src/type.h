
#include "tree.h"

/* NOTE:
 * Types that are supported in type checking
 */
TYPEK *intTYPE, *float64TYPE, *runeTYPE, *boolTYPE, *stringTYPE;

/* NOTE:
 * The following are declarations of functions that perform type checking
 */
void symImplementationIDL(IDL *id, EXP *e, SymbolTable *sym);
void symImplementationIDL2(IDL *id, DECLARATION *d, TYPE *t, SymbolTable *sym);
void checkFunctionExpressionFormal(FORMAL *f, EXP *e);
void checkAssignmentLeft(EXP *e, SymbolTable *sym);
void setVariableType3(EXP *e, IDL *id);
void checkArray(DECLARATION *d, EXP *e, SymbolTable *sym);
void iterateArray(DECLARATION *d, EXP *parent, EXP *recursive, TYPEK *k, SymbolTable *sym);
void initBaseTypes();

/* NOTE:
 * Helper functions
 */
void checkVariableDeclaration(TYPE *t, EXP *exps);
void checkReturn(EXP *e, TYPE *t);
void checkAssignmentType(EXP *left, EXP *right, SymbolTable *sym);
void checkPrint(EXP *e, SymbolTable *sym);
void checkPrintln(EXP *e, SymbolTable *sym);
void checkIncrement(EXP *e, SymbolTable *sym);
void checkDecrement(EXP *e, SymbolTable *sym);
void checkIfCondition(EXP *e);
void checkSwitch(EXP *e, STATEMENT *clause, SymbolTable *sym);
void checkSwitch1(EXP *e, STATEMENT *clause);
void checkSwitch2(STATEMENT *clause);
void checkCase1(EXP *expr, EXP *e);
void checkCase2(EXP *e);
void checkFor(EXP *e);
void checkUplus(EXP *e);
void checkUminus(EXP *e);
void checkUnot(EXP *e);
void checkUxor(EXP *e);
void checkTor(EXP *par, EXP *left, EXP *right);
void checkTand(EXP *par, EXP *left, EXP *right);
void checkEq(EXP *par, EXP *left, EXP *right, SymbolTable *sym);
void checkNeq(EXP *par, EXP *left, EXP *right, SymbolTable *sym);
void checkL(EXP *par, EXP *left, EXP *right);
void checkLeq(EXP *par, EXP *left, EXP *right);
void checkG(EXP *par, EXP *left, EXP *right);
void checkGeq(EXP *par, EXP *left, EXP *right);
void checkPlus(EXP *par, EXP *left, EXP *right);
void checkMinus(EXP *par, EXP *left, EXP *right);
void checkOr(EXP *par, EXP *left, EXP *right);
void checkXor(EXP *par, EXP *left, EXP *right);
void checkMultiply(EXP *par, EXP *left, EXP *right);
void checkDivide(EXP *par, EXP *left, EXP *right);
void checkMod(EXP *par, EXP *left, EXP *right);
void checkLshift(EXP *par, EXP *left, EXP *right);
void checkRshift(EXP *par, EXP *left, EXP *right);
void checkAnd(EXP *par, EXP *left, EXP *right);
void checkNand(EXP *par, EXP *left, EXP *right);
void checkSpecialFunctions(DECLARATION *d);
void checkInvalidFormal(FORMAL *f);
void checkInvalidReturn(TYPE *t);
void checkSpecialFunctionScope(EXP *e);
void checkTypeSymCast(TYPE *castType, EXP *e, SymbolTable *sym);
void checkDeclarationFct(EXP *e, SymbolTable *sym);
void checkArrayDeclaration(TYPE *t, EXP *e, SymbolTable *sym);
void compareArrayType(TYPE *t1, TYPE *t2);
void filterRecursive(IDL *id, TYPE *t, SymbolTable *sym);
void filterInit(IDL *id);
void filterExpressionStatement(EXP *e, SymbolTable *sym);
void checkIndexLeft(EXP *e, SymbolTable *sym);
void checkPreDeclaration(int parent, int child);
int countCast(EXP *e);





